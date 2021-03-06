
#include "private.h"
#include <udjat/request.h>
#include <udjat/agent.h>
#include <udjat/service.h>
#include <cstring>
#include <civetweb.h>
#include <json/value.h>
#include <udjat/worker.h>

#ifdef HAVE_CIVETWEB

static int log_message(const struct mg_connection *conn, const char *message) {
	clog << message << endl;
	return 1;
}

static int WebHandler(struct mg_connection *conn, void UDJAT_UNUSED(*cbdata)) {

	const struct mg_request_info *ri = mg_get_request_info(conn);

	if(strcasecmp(ri->request_method,"get")) {
		mg_send_http_error(conn, 405, "Invalid request method");
		return 405;
	}

	Response	response;

	try {

		cout << "URI: '" << (ri->local_uri + 5) << "'" << endl;

		const char * ptr = strchr(ri->local_uri + 5,'/');
		if(!ptr)
			throw system_error(ENOENT,system_category(),"Invalid request");

		ptr++;

		const char * path = strchr(ptr,'/');
		string cmd;

		if(!path)
			path = strchr(ptr,'?');

		if(path) {
			cmd.assign(ptr,path-ptr);
		} else {
			path = "";
			cmd.assign(ptr);
		}

		cout << "CMD: '" << cmd << "' path: '" << path << "'" << endl;

		Worker::work(cmd.c_str(),Request(path),response);

	} catch(const exception &e) {

		mg_send_http_error(conn, 500, e.what());
		return 500;

	}

	string rsp = response.toStyledString();

	cout << "Response:" << endl << rsp << endl;

	mg_send_http_ok(conn, "application/json; charset=utf-8", rsp.size());
	mg_write(conn, rsp.c_str(), rsp.size());

	return 200;

}

void run_civetweb() {

	cout << "Starting civetweb server" << endl;

	// https://github.com/civetweb/civetweb/blob/master/docs/UserManual.md
	static const char *port = "8989";
	static const char *options[] = {
		"listening_ports", 			port,
		"request_timeout_ms",		"10000",
		"error_log_file",			"error.log",
		"enable_auth_domain_check",	"no",
		NULL
	};

	struct mg_callbacks callbacks;
	memset(&callbacks,0,sizeof(callbacks));
	callbacks.log_message = log_message;

	mg_init_library(0);

	struct mg_context *ctx = mg_start(&callbacks, 0, options);

	if (ctx == NULL) {
		throw runtime_error("Cannot start CivetWeb - mg_start failed.");
	}

	mg_set_request_handler(ctx, "/api/", WebHandler, 0);

	cout	<< "http://127.0.0.1:" << port << "/api/" << PACKAGE_VERSION "." PACKAGE_RELEASE << "/agent" << endl
			<< "http://127.0.0.1:" << port << "/api/" << PACKAGE_VERSION "." PACKAGE_RELEASE << "/agent/intvalue" << endl
			<< "http://127.0.0.1:" << port << "/api/" << PACKAGE_VERSION "." PACKAGE_RELEASE << "/states" << endl
			<< endl;

	Service::run();

	mg_stop(ctx);

}


#endif // HAVE_CIVETWEB

