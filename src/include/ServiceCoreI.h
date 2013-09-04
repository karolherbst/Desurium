/*
Desura is the leading indie game distribution platform
Copyright (C) 2011 Mark Chandler (Desura Net Pty Ltd)

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>
*/

#ifndef DESURA_SERVICECOREI_H
#define DESURA_SERVICECOREI_H
#ifdef _WIN32
#pragma once
#endif

#define SERVICE_CORE "service_core_001"

#ifdef WITH_BREAKPAD
typedef void (*CrashSettingFn)(const wchar_t*, bool);
#endif
typedef void (*DisconnectFn)();

//! Service core handles all the tasks that need administraton rights to perform
class ServiceCoreI
{
public:
	//! Set up da pipe to start communications from the client
	//!
	virtual void startPipe()=0;
	
	//! Stop da pipe
	//!	
	virtual void stopPipe()=0;
	
	//! Stop windows service
	//!
	virtual bool stopService(const char* serviceName)=0;

	//! Destory this instance
	//!
	virtual void destroy()=0;

#ifdef WITH_BREAKPAD
	//! Set the crash settings callback
	//!
	virtual void setCrashSettingCallback(CrashSettingFn crashSettingFn)=0;
#endif

	//! Set the disconnect callback
	//!
	virtual void setDisconnectCallback(DisconnectFn disconnectFn)=0;
};

#endif //DESURA_SERVICECOREI_H
