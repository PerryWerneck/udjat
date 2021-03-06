/**
 * @file
 *
 * @brief Implements the agent update methods.
 *
 * @author perry.werneck@gmail.com
 *
 */

 #include "private.h"
 #include <udjat/event.h>
 #include <udjat/notification.h>
 #include <udjat/tools/threadpool.h>

 using namespace std;

//---[ Implement ]------------------------------------------------------------------------------------------

namespace Udjat {

	bool Abstract::Agent::chk4refresh(bool forward) {

		// Return if update is running.
		if(update.running || (update.expires && update.expires > time(nullptr)))
			return false;

		if(!update.on_demand) {

			// It's not on-demand, check for timer and return if still waiting
			if(update.next && update.next > time(nullptr))
				return false;

		}

		update.running = time(nullptr);

		if(update.timer) {
			update.next = update.expires = (update.running + update.timer);
		} else {
			update.next = 0;
			update.expires = update.running + 10;	// TODO: Make it configurable.
		}

		try {

			refresh();
			update.last = time(nullptr);

		} catch(const std::exception &e) {

			update.running = 0;
			failed(e,"Error updating agent");
			throw;

		} catch(...) {

			update.running = 0;
			failed("Unexpected error updating agent");
			throw;

		}

		if(forward) {
			// Check children
			lock_guard<std::recursive_mutex> lock(guard);
			for(auto child : children) {
				child->chk4refresh(true);
			}
		}

		update.running = 0;

		return true;

	}

	void Abstract::Agent::refresh() {

	}

	bool Abstract::Agent::hasOwnStates() const noexcept {
		return false;
	}

	void Abstract::Agent::onValueChange() noexcept {

		bool level_has_changed = false;

		// Compute new state
		try {

			// First get state for current agent value.
			auto new_state = find_state();

			// Does any children has worst state? if yes; use-it.
			{
				lock_guard<std::recursive_mutex> lock(guard);
				for(auto child : children) {

					if(child->state->getLevel() > new_state->getLevel()) {
						new_state = child->state;
					}
				}
			}

			level_has_changed = activate(new_state);

		} catch(const exception &e) {

			failed(e,"Error switching state");

		} catch(...) {

			failed("Unexpected error switching state");

		}

		// Fire events
		for(auto event : events) {

			try {

				event->set(*this,level_has_changed);

			} catch(const std::exception &e) {

				error("Error '{}' firing event '{}'",e.what(),event->c_str());

			} catch(...) {

				error("Unexpected error firing event '{}'",event->c_str());

			}

		}

		// Anyone looking for notifications?
		if(update.notify && Notification::hasListeners()) {

			class Notification : public Udjat::Notification {
			public:
				Notification(const Agent &agent) {

					label = agent.label;
					if(agent.state) {

						level = agent.state->getLevel();
						summary = agent.state->getSummary();
						message = agent.state->getBody();
						uri = agent.state->getUri();
						if(!uri)
							uri = agent.uri;

					} else {

						static const Quark msg = Quark::getFromStatic("Agent value has changed");
						summary = msg;
						uri = agent.uri;

					}

				}
			};

			try {

				Notification(*this).emit();

			} catch(const std::exception &e) {

				error("Error '{}' emiting notification",e.what());

			} catch(...) {

				error("Error '{}' emiting notification","unexpected");

			}


		}

	}

}
