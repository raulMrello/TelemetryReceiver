/*
# Created by Fabrizio Di Vittorio (fdivitto2013@gmail.com)
# Copyright (c) 2015 Fabrizio Di Vittorio.
# All rights reserved.

# GNU GPL LICENSE
#
# This module is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License as
# published by the Free Software Foundation; latest version thereof,
# available at: <http://www.gnu.org/licenses/gpl.txt>.
#
# This module is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this module; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307, USA
*/

// disable macros like "read"
#ifndef LWIP_POSIX_SOCKETS_IO_NAMES
#define LWIP_POSIX_SOCKETS_IO_NAMES 0
#endif

extern "C" {
    #include "esp_common.h"    
    #include "freertos/FreeRTOS.h"
    #include "freertos/task.h"
}

#include "fdvserial.h"
#include "fdvsync.h"
#include "fdvflash.h"
#include "fdvtask.h"
#include "fdvnetwork.h"


struct MyTCPConnectionHandler : public fdv::TCPConnectionHandler {
	void ICACHE_FLASH_ATTR connectionHandler() 	{
		while (isConnected()) {
			printf("C.free stack=%d bytes\n\r", fdv::Task::getFreeStack());
			printf("Waiting for data\n\r");
			char buffer[64];
			int32_t len = read(buffer, sizeof(buffer));
			if (len > 0) {
				printf("%d -> ", len);
				char const* data = buffer;
				while (len--)
					printf("%c", *data++);
				printf("\n\r");
				write("ok\n\r", 4);
			}
		}
		printf("disconnected\n\r");
	}
};




struct MainTask : fdv::Task {
	void ICACHE_FLASH_ATTR exec() {
		//fdv::DisableStdOut(); 
		fdv::DisableWatchDog();
		
		fdv::HardwareSerial serial(115200, 128);
		serial.printf(FSTR("\n\rTelemetry receiver started.\n\r"));
		
		// Access point setup with dhcp server
		fdv::WiFi::setMode(fdv::WiFi::AccessPoint);
		fdv::WiFi::configureAccessPoint("QC_wifi", "r231190m", 9);
		fdv::IP::configureStatic(fdv::IP::AccessPointNetwork, "192.168.5.1", "255.255.255.0", "192.168.5.1");
		fdv::DHCPServer::configure("192.168.5.100", "192.168.5.110", 10);
		serial.printf(FSTR("AccessPoint ssid: QC_wifi, pass: r231190m, channel: 9 \n\r"));

		// starts TCP server
		new fdv::TCPServer<MyTCPConnectionHandler>(80);
		
		suspend();
	}
};


extern "C" void /*ICACHE_FLASH_ATTR*/ user_init(void) {
	new MainTask;	// never destroy!
}

