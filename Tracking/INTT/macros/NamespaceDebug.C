R__LOAD_LIBRARY(libintt.so)
#include <intt/InttMapping.h>

#include <iostream>

void NamespaceDebug()
{
	bool b = false;
	struct Intt::RawData_s rawdata;
	struct Intt::Online_s online;
	struct Intt::Offline_s offline;

	struct Intt::RawData_s rawdata_temp;
	struct Intt::Online_s online_temp;
	struct Intt::Offline_s offline_temp;

	online = (struct Intt::Online_s){.lyr = 0, .ldr = 0, .arm = 0, .chp = 0, .chn = 0};
	b = false;
	while(true)
	{
		offline = ToOffline(online);

		b = (online != ToOnline(offline));
		if(b)
		{
			printf("\n");
			printf("%16s%2d\n", "lyr:", online.lyr);
			printf("%16s%2d\n", "ldr:", online.ldr);
			printf("%16s%2d\n", "arm:", online.arm);
			printf("%16s%2d\n", "chp:", online.chp);
			printf("%16s%2d\n", "chn:", online.chn);

			printf("%16s%2d\n", "layer:", offline.layer);
			printf("%16s%2d\n", "ladder_phi:", offline.ladder_phi);
			printf("%16s%2d\n", "ladder_z:", offline.ladder_z);
			printf("%16s%2d\n", "strip_x:", offline.strip_x);
			printf("%16s%2d\n", "strip_y:", offline.strip_y);

			online_temp = ToOnline(offline);
			printf("%16s%2d\n", "lyr:", online_temp.lyr);
			printf("%16s%2d\n", "ldr:", online_temp.ldr);
			printf("%16s%2d\n", "arm:", online_temp.arm);
			printf("%16s%2d\n", "chp:", online_temp.chp);
			printf("%16s%2d\n", "chn:", online_temp.chn);

			if(b)break;
		}

		++online.chn;
		if(online.chn < 128)continue;
		online.chn = 0;

		++online.chp;
		if(online.chp < 26)continue;
		online.chp = 0;

		break;
	}
	if(b)	std::cout << "Offline-Online failure" << std::endl;
	else	std::cout << "Offline-Online success" << std::endl;

	rawdata = (struct Intt::RawData_s){.felix_server = 0, .felix_channel = 0, .chip = 0, .channel = 0};
	b = false;
	while(true)
	{
		online = ToOnline(rawdata);

		b = (rawdata != ToRawData(online));
		if(b)
		{
			printf("\n");
			printf("%16s%2d\n", "felix_server:", rawdata.felix_server);
			printf("%16s%2d\n", "felix_channel:", rawdata.felix_channel);
			printf("%16s%2d\n", "chip:", rawdata.chip);
			printf("%16s%2d\n", "channel:", rawdata.channel);

			printf("%16s%2d\n", "lyr:", online.lyr);
			printf("%16s%2d\n", "ldr:", online.ldr);
			printf("%16s%2d\n", "arm:", online.arm);
			printf("%16s%2d\n", "chp:", online.chp);
			printf("%16s%2d\n", "chn:", online.chn);

			rawdata_temp = ToRawData(online);
			printf("%16s%2d\n", "felix_server:", rawdata_temp.felix_server);
			printf("%16s%2d\n", "felix_channel:", rawdata_temp.felix_channel);
			printf("%16s%2d\n", "chip:", rawdata_temp.chip);
			printf("%16s%2d\n", "channel:", rawdata_temp.channel);
			printf("\n");

			if(b)break;
		}
		

		++rawdata.felix_channel;
		if(rawdata.felix_channel < 14)continue;
		rawdata.felix_channel = 0;

		++rawdata.felix_server;
		if(rawdata.felix_server < 8)continue;
		rawdata.felix_server = 0;

		break;
	}
	if(b)	std::cout << "RawData-Online failure" << std::endl;
	else	std::cout << "RawData-Online success" << std::endl;
}

