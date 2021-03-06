#pragma once

#include <config.h>
#include <iostream>
#include <udjat/agent.h>
#include <unordered_map>
#include <udjat/tools/quark.h>
#include <pugixml.hpp>
#include <udjat/worker.h>

#ifdef HAVE_UNISTD_H
	#include <unistd.h>
#endif // HAVE_UNISTD_H

using namespace std;

namespace Udjat {

	std::shared_ptr<Abstract::State> get_default_state();

	class Abstract::Agent::Controller : private Worker, Agent::Factory {
	private:

		std::shared_ptr<Abstract::Agent> root;

		Controller();

	public:
		static Controller & getInstance();

		void set(std::shared_ptr<Abstract::Agent> root);

		std::shared_ptr<Abstract::Agent> get() const;
		std::shared_ptr<Abstract::Agent> find(const char *path) const;

		void work(const Request &request, Response &response) const override;
		void parse(Abstract::Agent &parent, const pugi::xml_node &node) const override;


	};

}
