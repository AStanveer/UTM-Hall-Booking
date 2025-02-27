// UTM HALL MANAGEMENT SYSTEM
// Programming Technique II - Section 06
/*Team Members:
            Anjum Siddiqua Tanveer Siddiqui A23CS0289
            Fatema Junaed A23CS0016 
            Taqia Tahmida A23CS0026
*/ 

//Lecturer : Ts. Dr. Johanna Binti Ahmad 


#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <ctime>
#include <vector>
#include <set>

using namespace std;

const int MAX_compare = 100;

class Hall {
private:
    string name;
    string num;
    string tar_date;
    string venue;

public:
    Hall() {}
    Hall(string n, string no, string date, string v) : name(n), num(no), tar_date(date), venue(v) {}

    string getName() const { return name; }
    string getNum() const { return num; }
    string getDate() const { return tar_date; }
    string getVenue() const { return venue; }

    void displayBooking() const {
        cout << "Name: " << name << "\nPhone Number: " << num << "\nDate: " << tar_date << "\nVenue: " << venue << endl;
    }

    static void saveBooking(const Hall& booking) {
        ofstream file("compare.txt", ios::app);
        if (file.is_open()) {
            file << booking.getName() << '\t' << booking.getNum() << '\t' << booking.getDate() << '\t' << booking.getVenue() << endl;
            file.close();
        } else {
            cout << "Unable to open file." << endl;
        }
    }

    static int fetchcompare(Hall compare[]) {
        ifstream file("compare.txt");
        int count = 0;
        if (file.is_open()) {
            string line;
            while (getline(file, line) && count < MAX_compare) {
                stringstream ss(line);
                string name, num, date, venue;
                getline(ss, name, '\t');
                getline(ss, num, '\t');
                getline(ss, date, '\t');
                getline(ss, venue, '\t');
                compare[count] = Hall(name, num, date, venue);
                count++;
            }
            file.close();
        } else {
            cout << "Unable to open file." << endl;
        }
        return count;
    }

    static int fetchAvailability(Hall compare[], const string& filepath) {
        ifstream file(filepath);
        int count = 0;
        if (file.is_open()) {
            string line;
            while (getline(file, line) && count < MAX_compare) {
                stringstream ss(line);
                string name, num, date, venue;
                getline(ss, name, '\t');
                getline(ss, num, '\t');
                getline(ss, date, '\t');
                getline(ss, venue, '\t');
                compare[count] = Hall(name, num, date, venue);
                count++;
            }
            file.close();
        } else {
            cout << "Unable to open file." << endl;
        }
        return count;
    }

    static void viewUserDetails() {
        ifstream file("compare.txt");
        if (file.is_open()) {
            string line;
            set<pair<string, string> > userDetails;
            while (getline(file, line)) {
                stringstream ss(line);
                string name, num, date, venue;
                getline(ss, name, '\t');
                getline(ss, num, '\t');
                getline(ss, date, '\t');
                getline(ss, venue, '\t');
                userDetails.insert(make_pair(name, num));
            }
            file.close();
            
            for (const auto& user : userDetails) {
                cout << "Name: " << user.first << ", Phone Number: " << user.second << endl;
            }
        } else {
            cout << "Unable to open file." << endl;
        }
    }

    bool operator==(const string& otherNum) const {
        return this->num == otherNum;
    }
};

class Person {
protected:
    string name;
    string num;

public:
    Person() {}
    Person(string n, string no) : name(n), num(no) {}

    virtual void displayMenu() = 0;
};

class User : public Person {
public:
    User(string n, string no) : Person(n, no) {}

    void displayMenu() override {
        int choice;
        do {
            cout << "\033[1;36m"; 
            cout << "=================================================================" << endl;
            cout << "                             User Menu                           " << endl;
            cout << "=================================================================" << endl;
            cout << "\033[1;36m"; 
            cout << "1. Check Availability\n";
            cout << "2. Make Reservation\n";
            cout << "3. Cancel Reservation\n";
            cout << "4. Retrieve Booking Info\n";
            cout << "5. Main Menu\n";
            cout << "Enter your choice : ";
            cin >> choice;
            switch (choice) {
            case 1:
                checkAvailability();
                break;
            case 2:
                makeReservation();
                break;
            case 3:
                cancelReservation();
                break;
            case 4:
                retrieveBooking();
                break;
            case 5:
                return;  // Return to main menu
            default:
                cout << "Invalid choice. Please try again." << endl;
            }
        } while (choice != 5);
    }

    bool isValidDate(const string& dateStr) {
        int day, month, year;
        char delim;
        stringstream ss(dateStr);
        ss >> day >> delim >> month >> delim >> year;

        if (ss.fail() || delim != '-' || day < 1 || day > 31 || month < 1 || month > 12 || year < 2024) {
            return false;
        }

        time_t t = time(nullptr);
        tm* now = localtime(&t);
        tm bookingDate = {0};
        bookingDate.tm_mday = day;
        bookingDate.tm_mon = month - 1;
        bookingDate.tm_year = year - 1900;

        time_t bookingTime = mktime(&bookingDate);
        if (difftime(bookingTime, t) < 0) {
            return false;
        }

        double diffMonths = difftime(bookingTime, t) / (60 * 60 * 24 * 30);
        if (diffMonths > 3) {
            return false;
        }

        return true;
    }

    void checkAvailability() {
        cout << "\033[1;36m"; 
        cout << "=================================================================" << endl;
        cout << "                        Check Availability                       " << endl;
        cout << "=================================================================" << endl;

        string venue, date;
        cin.ignore();
        cout << "Enter the venue: ";
        getline(cin, venue);
        cout << "Enter the date (DD-MM-YYYY): ";
        cin >> date;

        if (!isValidDate(date)) {
            cout << "Invalid booking date." << endl;
            return;
        }

        Hall compare[MAX_compare];
        int count = Hall::fetchAvailability(compare, "compare.txt");

        bool available = true;
        for (int i = 0; i < count; ++i) {
            if (compare[i].getVenue() == venue && compare[i].getDate() == date) {
                cout << "Venue is not available on " << date << "." << endl;
                available = false;
                break;
            }
        }

        if (available) {
            cout << "Venue is available on " << date << ". You can proceed with booking." << endl;
        }
    }

    void makeReservation() {
        cout << "\033[1;36m"; 
        cout << "=================================================================" << endl;
        cout << "                       Make Reservation                         " << endl;
        cout << "=================================================================" << endl;

        string name, num, date, venue;
        cin.ignore();
        cout << "Enter your name: ";
        getline(cin, name);
        cout << "Enter your phone number: ";
        cin >> num;
         
        cout << "Enter the date (DD-MM-YYYY): ";
        cin >> date;

        if (!isValidDate(date)) {
            cout << "Invalid booking date." << endl;
            return;
        }

        
        cout << "Enter the venue: ";
        cin.ignore();
        getline(cin, venue);

        Hall booking(name, num, date, venue);
        Hall::saveBooking(booking);
        cout << "Booking successfully made!" << endl;
    }

    void cancelReservation() {
        cout << "\033[1;36m"; 
        cout << "=================================================================" << endl;
        cout << "                        Cancel Reservation                       " << endl;
        cout << "=================================================================" << endl;
        string num;
        cin.ignore();
        cout << "\nEnter your phone number: ";
        cin >> num;

        Hall compare[MAX_compare]; 
        int count = Hall::fetchcompare(compare);

        bool found = false;
        vector<Hall> updatedBookings;
        for (int i = 0; i < count; ++i) {
            if (compare[i].getNum() == num) {
                found = true;
            } else {
                updatedBookings.push_back(compare[i]);
            }
        }

        if (found) {
            ofstream file("compare.txt");
            if (file.is_open()) {
                for (const Hall& booking : updatedBookings) {
                    file << booking.getName() << '\t' << booking.getNum() << '\t' << booking.getDate() << '\t' << booking.getVenue() << endl;
                }
                file.close();
            }
            cout << "\nReservation cancelled!" << endl;
        } else {
            cout << "No booking found for the provided information." << endl;
        }
    }


    void retrieveBooking() {
        cout << "\033[1;36m"; 
        cout << "=================================================================" << endl;
        cout << "                   Retrieve Booking Information                 " << endl;
        cout << "=================================================================" << endl;

        string num;
        cin.ignore();
        cout << "Enter your phone number: ";
        cin >> num;

        Hall compare[MAX_compare];
        int count = Hall::fetchcompare(compare);

        bool found = false;
        for (int i = 0; i < count; ++i) {
            if (compare[i] == num) {
                cout << "Booking Details:" << endl;
                compare[i].displayBooking();
                found = true;
                break;
            }
        }

        if (!found) {
            cout << "No booking found for the provided information." << endl;
        }
    }
};

class Admin : public Person {
public:
    Admin(string n, string no) : Person(n, no) {}

    void displayMenu() override {
        int choice;
        do {
            cout << "\033[1;36m"; 
            cout << "=================================================================" << endl;
            cout << "                            Admin Menu                           " << endl;
            cout << "=================================================================" << endl;
            cout << "\033[1;36m"; 
            cout << "1. View User Details\n";
            cout << "2. View All Bookings\n";
            cout << "3. Main Menu\n";
            cout << "Enter your choice : ";
            cin >> choice;
            switch (choice) {
            case 1:
                Hall::viewUserDetails();
                break;
            case 2:
                viewAllBookings();
                break;
            case 3:
                return;  // Return to main menu
            default:
                cout << "Invalid choice. Please try again." << endl;
            }
        } while (choice != 3);
    }

    void viewAllBookings() {
        cout << "\033[1;36m"; 
        cout << "=================================================================" << endl;
        cout << "                           All Bookings                           " << endl;
        cout << "=================================================================" << endl;

        Hall compare[MAX_compare];
        int count = Hall::fetchcompare(compare);

        if (count == 0) {
            cout << "No bookings found." << endl;
        } else {
            for (int i = 0; i < count; ++i) {
                cout << "\nBooking " << i + 1 << ":" << endl;
                compare[i].displayBooking();
            }
        }
    }
};

class Login {
public:
    void userLogin();
    void adminLogin();

    void mainMenu() {
        int role;
        do {
            cout << "\033[1;36m";
            cout << "=================================================================" << endl;
            cout << "                         Welcome to Hall Booking System          " << endl;
            cout << "=================================================================" << endl;
            cout << "\033[1;36m";
            cout << "1. User Login\n";
            cout << "2. Admin Login\n";
            cout << "3. Exit\n";
            cout << "Enter your role : ";
            cin >> role;
            switch (role) {
                case 1:
                    userLogin();
                    break;
                case 2:
                    adminLogin();
                    break;
                case 3:
                    cout << "Exiting program. Goodbye!" << endl;
                    break;
                default:
                    cout << "Invalid choice. Please try again." << endl;
            }
        } while (role != 3);
    }
};

void Login::userLogin() {
    string name, num;
    cout << "\033[1;36m";
    cout << "=================================================================" << endl;
    cout << "                          User Login                             " << endl;
    cout << "=================================================================" << endl;
    cout << "\033[1;36m";
    cin.ignore();
    cout << "Enter your name: ";
    getline(cin,name);
    cout << "\nEnter your phone number: ";
    cin >> num;

    User user(name, num);
    user.displayMenu();
}

void Login::adminLogin() {
    string name, num;
    cout << "\033[1;36m";
    cout << "=================================================================" << endl;
    cout << "                          Admin Login                            " << endl;
    cout << "=================================================================" << endl;
    cout << "\033[1;36m";
    cin.ignore();
    cout << "Enter your name: ";
    getline(cin,name);
    cout << "\nEnter your phone number: ";
    cin >> num;

    Admin admin(name, num);
    if (name == "Admin1" && num == "1234"){
        admin.displayMenu();
    } else {
        cout << endl << "Invalid Admin Details" << endl;
    }
}

int main() {
    Login().mainMenu();
    return 0;
}