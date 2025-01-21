#include <libnotify/notify.h>
#include <iostream>

#include <fstream>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

#include <bits/stdc++.h> 
using namespace std; 

int lastMin = -1;

bool sendReminder(string title, string body) {
    const char* cTitle = title.c_str();
    const char* cBody = body.c_str();

    NotifyNotification* n = notify_notification_new (cTitle, cBody, "battery");
    notify_notification_set_timeout(n, 10000); // 10 seconds

    if (!notify_notification_show(n, 0)) 
    {
        std::cerr << "show has failed" << std::endl;
        return false;
    }
    return true;
}

tm* getLocalTime() {
    // First we get the time
    time_t now = time(0);

    // Then we convert to local time
    tm* localTime = localtime(&now);
    return localTime;
}

bool isTime(int h, int m) {

    tm* localTime = getLocalTime();

    // then we extract the hour and minute
    int hour = localTime->tm_hour;
    int min = localTime->tm_min;

    if (h == hour && m == min) {
        return true;
    } else {
        return false;
    }
}

void triggerReminders(json reminderData) {
    for (int i = 0; i < reminderData.size(); i++) {
        int rHour = reminderData[i]["time"]["hour"];
        int rMin = reminderData[i]["time"]["min"];
        if (isTime(rHour,rMin)) {
            std::cout << "Displaying reminder for: " << reminderData[i]["time"]["hour"] << ":" << reminderData[i]["time"]["min"] << std::endl;
            sendReminder(reminderData[i]["title"],reminderData[i]["body"]);
        }
    }
}

int main(int argc, char * argv[] ) 
{
    std::ifstream reminderFile("reminders.json");

    if(reminderFile.fail()) {
        std::cerr << "Couldent parse the reminders.json file, is it in this directory?" << std::endl;
        return -1;
    }
    json data = json::parse(reminderFile);

    notify_init("Forget, What?");

    while (true) {
        tm* localTime = getLocalTime();

        // then we extract thee minute
        int min = localTime->tm_min;
        // We only trigger reminders one time per minute
        if (min != lastMin) {
            triggerReminders(data);
        }
        lastMin = min;
    }

    return 0;
}