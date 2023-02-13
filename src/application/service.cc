/* SPDX-License-Identifier: LGPL-3.0-or-later */

/*
 * Copyright (C) 2022 Perry Werneck <perry.werneck@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

 #include <config.h>
 #include <private/service.h>
 #include <udjat/module.h>
 #include <udjat/tools/threadpool.h>

 using namespace std;
 using namespace Udjat;

//---[ Implement ]------------------------------------------------------------------------------------------

#ifdef DEBUG
::Service::Service() : SystemService{"./xml.d"} {
}
#else
::Service::Service() : SystemService{} {
}
#endif // DEBUG

::Service::~Service() {
	Udjat::Module::unload();
}

void ::Service::init() {

	SystemService::init();
	info() << "Running build " << STRINGIZE_VALUE_OF(BUILD_DATE) << endl;

}

void ::Service::deinit() {

	info() << "Deinitializing" << endl;

	ThreadPool::getInstance().wait(); // Just in case.
	SystemService::deinit();
	Module::unload();

}
