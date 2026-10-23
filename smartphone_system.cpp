/*
 * ============================================================
 *  PROJECT  : Smartphone Features System
 *  LANGUAGE : C++17
 *  PURPOSE  : OOP Subject Activity – demonstrates Encapsulation,
 *             Abstraction, Inheritance, and Polymorphism through
 *             a console-based smartphone simulator.
 * ============================================================
 *
 *  CLASS HIERARCHY
 *  ───────────────
 *  SmartphoneFeature   (abstract base)
 *    ├── CallingFeature
 *    ├── MessagingFeature
 *    ├── CameraFeature
 *    └── MusicFeature
 *
 *  Smartphone          (core phone state – Encapsulation)
 *  ApplicationManager  (installed apps catalog)
 *  main()              (program entry & menu driver)
 * ============================================================
 */

#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <limits>
#include <algorithm>
#include <sstream>

using namespace std;

// ============================================================
//  UTILITY HELPERS
// ============================================================

// Clears the input buffer after bad reads
void clearInput() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

// Prints a horizontal separator line
void printLine(char ch = '-', int len = 55) {
    cout << string(len, ch) << "\n";
}

// Pause until the user presses Enter
void pauseScreen() {
    cout << "\nPress Enter to continue...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

// ============================================================
//  CLASS: Smartphone   (ENCAPSULATION)
//  Manages brand, model, battery, power state, connectivity,
//  and sound mode through public interface methods.
// ============================================================
class Smartphone {
private:
    // --- Private Data Members (Encapsulation) ---
    string brand;
    string model;
    int    batteryLevel;    // 0 - 100 %
    bool   poweredOn;
    bool   wifiOn;
    bool   mobileDataOn;
    string soundMode;       // "Ring", "Vibrate", "Silent"

public:
    // Constructor - initialises the smartphone details
    Smartphone(const string& brand, const string& model,
               int battery = 85)
        : brand(brand), model(model),
          batteryLevel(battery),
          poweredOn(true),
          wifiOn(true),
          mobileDataOn(false),
          soundMode("Ring")
    {}

    // Getters
    string getBrand()      const { return brand; }
    string getModel()      const { return model; }
    int    getBattery()    const { return batteryLevel; }
    bool   isPoweredOn()   const { return poweredOn; }
    bool   isWifiOn()      const { return wifiOn; }
    bool   isMobileData()  const { return mobileDataOn; }
    string getSoundMode()  const { return soundMode; }

    // Battery helpers

    // Drain battery by 'amount' %, clamped at 0
    bool drainBattery(int amount) {
        if (!poweredOn) {
            cout << "  [!] Phone is powered off. Turn it on first.\n";
            return false;
        }
        if (batteryLevel == 0) {
            cout << "  [!] Battery is dead! Please charge the phone.\n";
            return false;
        }
        batteryLevel = max(0, batteryLevel - amount);
        if (batteryLevel <= 15 && batteryLevel > 0)
            cout << "  [WARNING] Low battery: " << batteryLevel << "% remaining!\n";
        if (batteryLevel == 0)
            cout << "  [!] Battery just died!\n";
        return true;
    }

    // Charge battery (simulated)
    void chargeBattery(int amount) {
        int before = batteryLevel;
        batteryLevel = min(100, batteryLevel + amount);
        cout << "  Charging... " << before << "% -> " << batteryLevel << "%\n";
        if (batteryLevel == 100)
            cout << "  Battery fully charged!\n";
    }

    // Power controls
    void powerOff() {
        if (!poweredOn) { cout << "  Phone is already off.\n"; return; }
        poweredOn = false;
        cout << "  Shutting down " << brand << " " << model << "...\n";
        cout << "  Phone is now OFF.\n";
    }

    void powerOn() {
        if (poweredOn) { cout << "  Phone is already on.\n"; return; }
        if (batteryLevel == 0) {
            cout << "  Cannot power on - battery is dead. Please charge.\n";
            return;
        }
        poweredOn = true;
        cout << "  Powering on " << brand << " " << model << "...\n";
        cout << "  Phone is now ON.\n";
    }

    void restart() {
        if (!poweredOn) { cout << "  Phone is off. Use Power On instead.\n"; return; }
        cout << "  Restarting " << brand << " " << model << "...\n";
        poweredOn = false;
        poweredOn = true;
        cout << "  Phone has restarted successfully.\n";
    }

    // Connectivity
    void toggleWifi() {
        if (!poweredOn) { cout << "  Phone is off.\n"; return; }
        wifiOn = !wifiOn;
        // If Wi-Fi turns on, disable mobile data (common phone logic)
        if (wifiOn && mobileDataOn) {
            mobileDataOn = false;
            cout << "  Mobile Data turned OFF automatically.\n";
        }
        cout << "  Wi-Fi is now " << (wifiOn ? "ON" : "OFF") << ".\n";
    }

    void toggleMobileData() {
        if (!poweredOn) { cout << "  Phone is off.\n"; return; }
        mobileDataOn = !mobileDataOn;
        // If Mobile Data turns on, disable Wi-Fi (common phone logic)
        if (mobileDataOn && wifiOn) {
            wifiOn = false;
            cout << "  Wi-Fi turned OFF automatically.\n";
        }
        cout << "  Mobile Data is now " << (mobileDataOn ? "ON" : "OFF") << ".\n";
    }

    // Sound mode
    void setSoundMode(const string& mode) {
        if (!poweredOn) { cout << "  Phone is off.\n"; return; }
        soundMode = mode;
        cout << "  Sound mode set to: " << mode << "\n";
    }

    // Status display
    void displayStatus() const {
        printLine('=');
        cout << "  SMARTPHONE STATUS\n";
        printLine('=');
        cout << "  Brand  : " << brand  << "\n"
             << "  Model  : " << model  << "\n"
             << "  Power  : " << (poweredOn ? "ON" : "OFF") << "\n"
             << "  Battery: " << batteryLevel << "%"
             << (batteryLevel <= 15 ? " [LOW]" : "") << "\n"
             << "  Wi-Fi  : " << (wifiOn        ? "ON" : "OFF") << "\n"
             << "  Data   : " << (mobileDataOn  ? "ON" : "OFF") << "\n"
             << "  Sound  : " << soundMode << "\n";
        printLine('=');
    }
};


// ============================================================
//  CLASS: SmartphoneFeature   (ABSTRACTION)
//  Abstract base class defining the common interface that every
//  smartphone feature must implement.
// ============================================================
class SmartphoneFeature {
protected:
    Smartphone& phone;   // Reference to the shared phone object

public:
    // Constructor - every feature needs access to the phone state
    explicit SmartphoneFeature(Smartphone& phone) : phone(phone) {}

    // Pure virtual function - must be overridden by derived classes
    virtual void execute() = 0;

    // Virtual destructor (good practice for polymorphic base classes)
    virtual ~SmartphoneFeature() {}
};


// ============================================================
//  CLASS: CallingFeature   (INHERITANCE + POLYMORPHISM)
//  Simulates making and ending phone calls.
// ============================================================
class CallingFeature : public SmartphoneFeature {
private:
    // Simple phone-number validator
    bool isValidNumber(const string& number) const {
        if (number.length() < 7 || number.length() > 15) return false;
        for (char c : number)
            if (!isdigit(c) && c != '+' && c != '-' && c != ' ')
                return false;
        return true;
    }

public:
    explicit CallingFeature(Smartphone& phone)
        : SmartphoneFeature(phone) {}

    // Overrides the pure virtual function (Polymorphism)
    void execute() override {
        printLine('=');
        cout << "  CALLING SYSTEM\n";
        printLine('=');

        if (!phone.isPoweredOn()) {
            cout << "  Phone is OFF. Turn it on to make calls.\n";
            pauseScreen(); return;
        }
        if (phone.getBattery() == 0) {
            cout << "  Battery dead. Cannot make a call.\n";
            pauseScreen(); return;
        }

        string number;
        cout << "  Enter phone number to call (or 0 to cancel): ";
        getline(cin, number);

        if (number == "0") { cout << "  Cancelled.\n"; pauseScreen(); return; }

        if (!isValidNumber(number)) {
            cout << "  [!] Invalid phone number. Please enter 7-15 digits.\n";
            pauseScreen(); return;
        }

        // Drain battery: calls cost 5%
        if (!phone.drainBattery(5)) { pauseScreen(); return; }

        cout << "\n  Calling " << number << "...\n";
        cout << "  ------------------------------------------\n";
        cout << "  [  Call in progress with " << number << "  ]\n";
        cout << "  ------------------------------------------\n";
        cout << "\n  Press Enter to end the call...";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        cout << "  Call ended. Duration: 00:01:23 (simulated)\n";
        cout << "  Battery used: 5%\n";
        pauseScreen();
    }
};


// ============================================================
//  CLASS: MessagingFeature   (INHERITANCE + POLYMORPHISM)
//  Simulates sending SMS messages and viewing sent history.
// ============================================================
class MessagingFeature : public SmartphoneFeature {
private:
    // Inner struct to represent a single message
    struct Message {
        string recipient;
        string content;
    };

    vector<Message> sentMessages;   // In-memory message history

    void sendMessage() {
        string recipient, content;

        cout << "  Recipient number: ";
        getline(cin, recipient);

        if (recipient.empty() || recipient == "0") {
            cout << "  Cancelled.\n"; return;
        }

        // Simple length validation
        if (recipient.length() < 7) {
            cout << "  [!] Invalid number.\n"; return;
        }

        cout << "  Message (max 160 chars): ";
        getline(cin, content);

        if (content.empty()) {
            cout << "  [!] Message cannot be empty.\n"; return;
        }

        // Truncate to 160 characters (SMS standard)
        if (content.length() > 160)
            content = content.substr(0, 160);

        // Drain battery: messages cost 2%
        if (!phone.drainBattery(2)) return;

        sentMessages.push_back({recipient, content});
        cout << "\n  Message sent to " << recipient << "!\n";
        cout << "  Battery used: 2%\n";
    }

    void viewMessages() const {
        printLine();
        cout << "  SENT MESSAGES (" << sentMessages.size() << " total)\n";
        printLine();
        if (sentMessages.empty()) {
            cout << "  No messages sent yet.\n";
            return;
        }
        for (int i = 0; i < (int)sentMessages.size(); ++i) {
            cout << "  [" << (i + 1) << "] To: " << sentMessages[i].recipient << "\n"
                 << "      \"" << sentMessages[i].content << "\"\n";
            printLine();
        }
    }

public:
    explicit MessagingFeature(Smartphone& phone)
        : SmartphoneFeature(phone) {}

    void execute() override {
        if (!phone.isPoweredOn()) {
            cout << "  Phone is OFF.\n"; pauseScreen(); return;
        }
        if (phone.getBattery() == 0) {
            cout << "  Battery dead.\n"; pauseScreen(); return;
        }

        int choice = 0;
        do {
            printLine('=');
            cout << "  MESSAGING SYSTEM\n";
            printLine('=');
            cout << "  1. Send a New Message\n"
                 << "  2. View Sent Messages\n"
                 << "  0. Back\n";
            printLine();
            cout << "  Choice: ";
            cin >> choice;
            clearInput();

            switch (choice) {
                case 1: sendMessage();   break;
                case 2: viewMessages();  break;
                case 0: break;
                default: cout << "  Invalid option.\n";
            }
            if (choice != 0) pauseScreen();
        } while (choice != 0);
    }
};


// ============================================================
//  CLASS: CameraFeature   (INHERITANCE + POLYMORPHISM)
//  Simulates capturing photos and viewing the photo list.
// ============================================================
class CameraFeature : public SmartphoneFeature {
private:
    vector<string> photos;   // Each entry is a simulated filename
    int photoCounter;

    void capturePhoto() {
        // Drain battery: photos cost 3%
        if (!phone.drainBattery(3)) return;

        // Generate a filename: IMG_0001.jpg, IMG_0002.jpg, ...
        ostringstream oss;
        oss << "IMG_" << setw(4) << setfill('0') << ++photoCounter << ".jpg";
        string filename = oss.str();

        photos.push_back(filename);
        cout << "\n  *Click!* Photo captured!\n";
        cout << "  Saved as: " << filename << "\n";
        cout << "  Total photos: " << photos.size() << "\n";
        cout << "  Battery used: 3%\n";
    }

    void viewPhotoList() const {
        printLine();
        cout << "  PHOTO GALLERY (" << photos.size() << " photos)\n";
        printLine();
        if (photos.empty()) {
            cout << "  No photos taken yet.\n"; return;
        }
        for (int i = 0; i < (int)photos.size(); ++i)
            cout << "  [" << (i + 1) << "] " << photos[i] << "\n";
    }

public:
    CameraFeature(Smartphone& phone)
        : SmartphoneFeature(phone), photoCounter(0) {}

    void execute() override {
        if (!phone.isPoweredOn()) {
            cout << "  Phone is OFF.\n"; pauseScreen(); return;
        }
        if (phone.getBattery() == 0) {
            cout << "  Battery dead.\n"; pauseScreen(); return;
        }

        int choice = 0;
        do {
            printLine('=');
            cout << "  CAMERA SYSTEM\n";
            printLine('=');
            cout << "  1. Capture Photo\n"
                 << "  2. View Photo Gallery\n"
                 << "  0. Back\n";
            printLine();
            cout << "  Choice: ";
            cin >> choice;
            clearInput();

            switch (choice) {
                case 1: capturePhoto();   break;
                case 2: viewPhotoList();  break;
                case 0: break;
                default: cout << "  Invalid option.\n";
            }
            if (choice != 0) pauseScreen();
        } while (choice != 0);
    }
};


// ============================================================
//  CLASS: MusicFeature   (INHERITANCE + POLYMORPHISM)
//  Simulates a music player with a predefined song list.
// ============================================================
class MusicFeature : public SmartphoneFeature {
private:
    vector<string> playlist;
    int  currentSongIndex;   // -1 = not playing
    bool isPlaying;

    void displayPlaylist() const {
        printLine();
        cout << "  PLAYLIST\n";
        printLine();
        for (int i = 0; i < (int)playlist.size(); ++i) {
            cout << "  " << (i + 1) << ". " << playlist[i];
            if (isPlaying && i == currentSongIndex)
                cout << "  << NOW PLAYING";
            cout << "\n";
        }
        printLine();
    }

    void playSong() {
        displayPlaylist();
        cout << "  Enter song number to play (0 to cancel): ";
        int choice;
        cin >> choice;
        clearInput();

        if (choice == 0) return;
        if (choice < 1 || choice > (int)playlist.size()) {
            cout << "  [!] Invalid song number.\n"; return;
        }

        // Drain battery: music costs 4%
        if (!phone.drainBattery(4)) return;

        currentSongIndex = choice - 1;
        isPlaying = true;
        cout << "\n  Now Playing: " << playlist[currentSongIndex] << "\n";
        cout << "  Battery used: 4%\n";
    }

    void stopPlayback() {
        if (!isPlaying) {
            cout << "  No song is currently playing.\n"; return;
        }
        cout << "  Stopped: " << playlist[currentSongIndex] << "\n";
        isPlaying = false;
        currentSongIndex = -1;
    }

public:
    MusicFeature(Smartphone& phone)
        : SmartphoneFeature(phone), currentSongIndex(-1), isPlaying(false)
    {
        // Predefined playlist
        playlist = {
            "Blinding Lights    - The Weeknd",
            "Shape of You       - Ed Sheeran",
            "Levitating         - Dua Lipa",
            "Stay               - The Kid LAROI & Justin Bieber",
            "Dynamite           - BTS",
            "Peaches            - Justin Bieber",
            "Good 4 U           - Olivia Rodrigo",
            "Montero            - Lil Nas X",
            "Watermelon Sugar   - Harry Styles",
            "Save Your Tears    - The Weeknd"
        };
    }

    void execute() override {
        if (!phone.isPoweredOn()) {
            cout << "  Phone is OFF.\n"; pauseScreen(); return;
        }
        if (phone.getBattery() == 0) {
            cout << "  Battery dead.\n"; pauseScreen(); return;
        }

        int choice = 0;
        do {
            printLine('=');
            cout << "  MUSIC PLAYER";
            if (isPlaying)
                cout << "  [ >> " << playlist[currentSongIndex] << " ]";
            cout << "\n";
            printLine('=');
            cout << "  1. View Playlist & Play a Song\n"
                 << "  2. Stop Playback\n"
                 << "  0. Back\n";
            printLine();
            cout << "  Choice: ";
            cin >> choice;
            clearInput();

            switch (choice) {
                case 1: playSong();      break;
                case 2: stopPlayback();  break;
                case 0: break;
                default: cout << "  Invalid option.\n";
            }
            if (choice != 0) pauseScreen();
        } while (choice != 0);
    }
};


// ============================================================
//  CLASS: ApplicationManager
//  Manages a catalog of installed apps and their descriptions.
// ============================================================
class ApplicationManager {
private:
    // Inner struct for an application entry
    struct App {
        string name;
        string description;
    };

    vector<App> apps;

public:
    ApplicationManager() {
        // Predefined list of installed applications
        apps = {
            { "Phone",    "Make and receive voice calls. Access your recent call log." },
            { "Messages", "Send and receive SMS text messages with contacts." },
            { "Camera",   "Capture photos and videos. Manage your media gallery." },
            { "Music",    "Play your favourite songs and manage playlists." },
            { "Settings", "Adjust Wi-Fi, Bluetooth, display, sound, and privacy." },
            { "Gallery",  "Browse all photos and videos stored on the device." },
            { "Maps",     "Navigate with turn-by-turn directions and live traffic." },
            { "Clock",    "Set alarms, use the stopwatch, and manage timers." }
        };
    }

    void displayApps() const {
        printLine('=');
        cout << "  INSTALLED APPLICATIONS\n";
        printLine('=');
        for (int i = 0; i < (int)apps.size(); ++i)
            cout << "  " << (i + 1) << ". " << apps[i].name << "\n";
        printLine();
        cout << "  Select an app number to see details (0 to go back): ";
    }

    void viewApp(int index) const {
        if (index < 1 || index > (int)apps.size()) {
            cout << "  [!] Invalid selection.\n"; return;
        }
        const App& a = apps[index - 1];
        printLine();
        cout << "  App     : " << a.name << "\n"
             << "  About   : " << a.description << "\n";
        printLine();
    }

    int getAppCount() const { return (int)apps.size(); }
};


// ============================================================
//  FREE FUNCTION: connectivityMenu
//  Handles the Connectivity Settings sub-menu.
// ============================================================
void connectivityMenu(Smartphone& phone) {
    int choice = 0;
    do {
        printLine('=');
        cout << "  CONNECTIVITY SETTINGS\n";
        printLine('=');
        cout << "  Current Status:\n"
             << "    Wi-Fi       : " << (phone.isWifiOn()    ? "ON" : "OFF") << "\n"
             << "    Mobile Data : " << (phone.isMobileData() ? "ON" : "OFF") << "\n";
        printLine();
        cout << "  1. Toggle Wi-Fi\n"
             << "  2. Toggle Mobile Data\n"
             << "  0. Back\n";
        printLine();
        cout << "  Choice: ";
        cin >> choice;
        clearInput();

        switch (choice) {
            case 1: phone.toggleWifi();        break;
            case 2: phone.toggleMobileData();  break;
            case 0: break;
            default: cout << "  Invalid option.\n";
        }
        if (choice != 0) pauseScreen();
    } while (choice != 0);
}


// ============================================================
//  FREE FUNCTION: batteryMenu
//  Handles the Battery Management sub-menu.
// ============================================================
void batteryMenu(Smartphone& phone) {
    int choice = 0;
    do {
        printLine('=');
        cout << "  BATTERY MANAGEMENT\n";
        printLine('=');
        cout << "  Current Battery: " << phone.getBattery() << "%\n";
        printLine();
        cout << "  1. Charge Battery (+20%)\n"
             << "  2. Charge Battery (+50%)\n"
             << "  3. Charge to 100%\n"
             << "  0. Back\n";
        printLine();
        cout << "  Choice: ";
        cin >> choice;
        clearInput();

        switch (choice) {
            case 1: phone.chargeBattery(20);   break;
            case 2: phone.chargeBattery(50);   break;
            case 3: phone.chargeBattery(100);  break;
            case 0: break;
            default: cout << "  Invalid option.\n";
        }
        if (choice != 0) pauseScreen();
    } while (choice != 0);
}


// ============================================================
//  FREE FUNCTION: soundMenu
//  Handles the Sound Mode sub-menu.
// ============================================================
void soundMenu(Smartphone& phone) {
    int choice = 0;
    do {
        printLine('=');
        cout << "  SOUND SETTINGS\n";
        printLine('=');
        cout << "  Current Mode: " << phone.getSoundMode() << "\n";
        printLine();
        cout << "  1. Ring\n"
             << "  2. Vibrate\n"
             << "  3. Silent\n"
             << "  0. Back\n";
        printLine();
        cout << "  Choice: ";
        cin >> choice;
        clearInput();

        switch (choice) {
            case 1: phone.setSoundMode("Ring");    break;
            case 2: phone.setSoundMode("Vibrate"); break;
            case 3: phone.setSoundMode("Silent");  break;
            case 0: break;
            default: cout << "  Invalid option.\n";
        }
        if (choice != 0) pauseScreen();
    } while (choice != 0);
}


// ============================================================
//  FREE FUNCTION: powerMenu
//  Handles power-related options: shutdown, restart, power-on.
// ============================================================
void powerMenu(Smartphone& phone) {
    int choice = 0;
    do {
        printLine('=');
        cout << "  POWER OPTIONS\n";
        printLine('=');
        cout << "  Phone is currently: " << (phone.isPoweredOn() ? "ON" : "OFF") << "\n";
        printLine();
        cout << "  1. Power Off\n"
             << "  2. Restart\n"
             << "  3. Power On\n"
             << "  0. Back\n";
        printLine();
        cout << "  Choice: ";
        cin >> choice;
        clearInput();

        switch (choice) {
            case 1: phone.powerOff();  break;
            case 2: phone.restart();   break;
            case 3: phone.powerOn();   break;
            case 0: break;
            default: cout << "  Invalid option.\n";
        }
        if (choice != 0) pauseScreen();
    } while (choice != 0);
}


// ============================================================
//  MAIN - Program entry point
// ============================================================
int main() {
    // Create the simulated smartphone (Encapsulation)
    Smartphone phone("Samsung", "Galaxy S24", 85);

    // Create feature objects (Inheritance)
    // Stored as base-class pointers to enable Polymorphism
    CallingFeature   calling(phone);
    MessagingFeature messaging(phone);
    CameraFeature    camera(phone);
    MusicFeature     music(phone);

    // Feature pointer array - demonstrates Polymorphism
    // through a unified SmartphoneFeature* interface
    SmartphoneFeature* features[] = {
        &calling,
        &messaging,
        &camera,
        &music
    };

    // Application manager
    ApplicationManager appManager;

    // Main menu loop
    int choice = -1;
    while (choice != 0) {
        cout << "\n";
        printLine('=');
        cout << "  SMARTPHONE FEATURES SYSTEM\n";
        cout << "  " << phone.getBrand() << " " << phone.getModel()
             << "  |  Battery: " << phone.getBattery() << "%"
             << "  |  " << (phone.isPoweredOn() ? "ON" : "OFF") << "\n";
        printLine('=');
        cout << "  1.  Smartphone Status\n"
             << "  2.  Calling System\n"
             << "  3.  Messaging System\n"
             << "  4.  Camera System\n"
             << "  5.  Connectivity Settings\n"
             << "  6.  Music Player\n"
             << "  7.  Installed Applications\n"
             << "  8.  Battery Management\n"
             << "  9.  Sound Settings\n"
             << "  10. Power Options\n"
             << "  0.  Exit\n";
        printLine('=');
        cout << "  Enter your choice: ";

        cin >> choice;
        clearInput();
        cout << "\n";

        switch (choice) {
            case 1:
                // Display full phone status
                phone.displayStatus();
                pauseScreen();
                break;

            case 2:
                // Polymorphic call: CallingFeature::execute()
                features[0]->execute();
                break;

            case 3:
                // Polymorphic call: MessagingFeature::execute()
                features[1]->execute();
                break;

            case 4:
                // Polymorphic call: CameraFeature::execute()
                features[2]->execute();
                break;

            case 5:
                connectivityMenu(phone);
                break;

            case 6:
                // Polymorphic call: MusicFeature::execute()
                features[3]->execute();
                break;

            case 7: {
                // Application manager sub-menu
                int appChoice = -1;
                do {
                    appManager.displayApps();
                    cin >> appChoice;
                    clearInput();
                    if (appChoice != 0)
                        appManager.viewApp(appChoice);
                    if (appChoice != 0) pauseScreen();
                } while (appChoice != 0);
                break;
            }

            case 8:
                batteryMenu(phone);
                break;

            case 9:
                soundMenu(phone);
                break;

            case 10:
                powerMenu(phone);
                break;

            case 0:
                printLine('=');
                cout << "  Exiting Smartphone Features System.\n"
                     << "  Thank you for using " << phone.getBrand()
                     << " " << phone.getModel() << "!\n";
                printLine('=');
                break;

            default:
                cout << "  [!] Invalid choice. Please enter 0-10.\n";
                pauseScreen();
        }
    }

    return 0;
}
