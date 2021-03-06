/**
 * @file
 *
 * @brief Implements the agent state machine.
 *
 * @author perry.werneck@gmail.com
 *
 */

 #include "private.h"
 #include <udjat/tools/configuration.h>

//---[ Implement ]------------------------------------------------------------------------------------------

namespace Udjat {

	std::shared_ptr<Abstract::State> get_default_state() {

		class DefaultState : public Abstract::State, Abstract::Agent::Factory {
		public:
			DefaultState() : Abstract::State(Abstract::State::undefined,""), Abstract::Agent::Factory(I_("state")) {
#ifdef DEBUG
				cout << "Default state was created" << endl;
#endif // DEBUG
			}

			~DefaultState() {
#ifdef DEBUG
				cout << "Default state was destroyed" << endl;
#endif // DEBUG
			}

			void parse(Abstract::Agent &agent, const pugi::xml_node &node) const {

#ifdef DEBUG
				cout << "Parsing state '" << "' for agent '" << agent.getName() << "'" << endl;
#endif // DEBUG

				agent.append_state(node);

			}

		};

		static shared_ptr<Abstract::State> state(new DefaultState());
		return state;

	}

	static time_t getDelayAfterException() noexcept {

		try {

			return Config::File::getInstance().get("delay","after_exception",10);

		} catch(const exception &e) {

			cerr << PACKAGE_NAME << "\t" << e.what() << endl;

		}

		return 10;
	}

	/// @brief Activate an error state.
	void Abstract::Agent::failed(const std::exception &e, const char *message) noexcept {

		cerr << *this << "\t" << message << ": " << e.what() << endl;
		this->state = make_shared<State>(State::critical,message,e.what());
		this->update.next = time(nullptr) + getDelayAfterException();

		if(parent)
			parent->onChildStateChange();

	}

	/// @brief Set failed state from known exception
	void Abstract::Agent::failed(const char *message) noexcept {

		cerr << *this << "\t" << message << endl;
		this->state = make_shared<State>(State::critical,message);
		this->update.next = time(nullptr) + getDelayAfterException();

		if(parent)
			parent->onChildStateChange();

	}

	void Abstract::Agent::onChildStateChange() noexcept {

		try {

			// Compute my current state based on value.
			auto new_state = find_state();

			// Then check the children.
			{
				lock_guard<std::recursive_mutex> lock(guard);
				for(auto child : children) {

					if(child->state->getLevel() > new_state->getLevel()) {
						new_state = child->state;
					}
				}
			}

			activate(new_state);

		} catch(const std::exception &e) {

			failed(e,"Error switching state");

		} catch(...) {

			failed("Unexpected error switching state");

		}


	}

	bool Abstract::Agent::activate(std::shared_ptr<State> state) noexcept {

		// It's an empty state? If yes replaces with the default one.
		if(!state)
			state = Abstract::Agent::find_state();

		// Return if it's the same.
		if(state == this->state)
			return false;

		info("Current state changes from '{}' to '{}'",
				this->state->getSummary(),
				state->getSummary()
			);

		State::Level saved_state = this->state->getLevel();

		try {

			this->state->deactivate(*this);
			this->state = state;
			this->state->activate(*this);

			if(parent)
				parent->onChildStateChange();

		} catch(const std::exception &e) {

			failed(e,"Error switching state");

		} catch(...) {

			failed("Unexpected error switching state");

		}

#ifdef DEBUG
		if(saved_state != this->state->getLevel()) {
			info("State has changed from '{}' to '{}'",saved_state,this->state->getLevel());
		}
#endif // DEBUG

		return (saved_state != this->state->getLevel());
	}

}
