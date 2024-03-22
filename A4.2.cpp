/***********************************************************************
Matr.-Nr:                     3085596
Nachname/Surname:             Haque
Vorname/Given name:           Md Anamul
Uni-Email:                    md.haque.696@stud.uni-due.de
Studiengang/Course of studis: ISE(Software Engineering)
***********************************************************************/
#include<iostream>
#include<string>
#include<iomanip>
#include<fstream>
#include<sstream>
using namespace std;

const int annualAverageConsumptionPerPerson = 200;
const int annualAverageConsumptionPerSquareMeter = 9;
const int annual_average_consumption_per_person_with_electric_water_heating = 550;

// 1. Define a enumeration structure called use
enum Use
{
    once,
    daily,
    mo_fr,
    sa_su,
    weekly
};

// 2. Function to input frequency of use
void input_use(string c, Use& use)
{
    cout << "how often it will be used?";
    cout << "(d) daily";
    cout << "(m) mo_fr";
    cout << "(o) once";
    cout << "(s) sa_su";
    cout << "(w) weekly";

    cout << c;
    char choice;
    cin >> choice;

    switch (choice)
    {
    case 'd': use = daily;  break;
    case 'm': use = mo_fr;  break;
    case 'o': use = once;   break;
    case 's': use = sa_su;  break;
    case 'w': use = weekly; break;
    default:
        cout << "Invalid choice. Using 'once' as default." << endl;
        use = once;
        break;
    }
}

// 3. Define a structure called Consumer
struct consumer
{
    string description;
    float watt;
    float watt_standby;
    float hours;
    Use use;
    consumer* next;
};

// 4. Difine a Extend structer household with pointer Consumer
struct Household
{
    string city;
    int numPersons;
    int squareMeters;
    bool Hot_water;
    consumer* consumers;
    float powerPrice;
    string powerSupplier;
};


// from task A1
float calculateAnnualPowerConsumption(Household& house)
{
    float annualPowerConsumptionPerPerson = house.Hot_water ? annual_average_consumption_per_person_with_electric_water_heating : annualAverageConsumptionPerPerson;
    return ((house.numPersons * annualPowerConsumptionPerPerson) + (house.squareMeters * annualAverageConsumptionPerSquareMeter));
}



// From task A1
float calculateAnnualPowerCosts(Household& house, float pricePerKWh)
{
    return calculateAnnualPowerConsumption(house) * pricePerKWh;
}

// 5. Function to add new Consumer in Household
void addConsumerToHousehold(Household* house, consumer* newConsumer)
{
    if (!house->consumers)
    {
        house->consumers = newConsumer;
    }
    else
    {
        consumer* temp = house->consumers;
        while (temp->next) temp = temp->next;
        temp->next = newConsumer;
    }
}

// 6. Function to calculate annual hours for each Consumer
float annualHoursOfUse(consumer* cons)
{
    switch (cons->use)
    {
        case once: return cons->hours;
        case daily: return cons->hours * 365;
        case mo_fr: return cons->hours * 260;
        case sa_su: return cons->hours * 104;
        case weekly: return cons->hours * 52;
        default: return 0;
    }
}

// 7. Function to calcualte annual hours standby
float annualHoursOfStandby(consumer* cons)
{
    return 8760 - annualHoursOfUse(cons);
}

// 8. Function to calculate annual kwh
float annualKWh(consumer* cons)
{
    return (((annualHoursOfUse(cons) * cons->watt)
        + (annualHoursOfStandby(cons) * cons->watt_standby)) / 1000);
}


// 9. move_up power consumer from consumer list
consumer* move_up(consumer* Current_Head, int k)
{
    if (k <= 1)
    {
        return Current_Head;
    }

    else
    {
        consumer* current = Current_Head;
        consumer* next_consumer = current->next;

        if (k == 2 && next_consumer != nullptr)
        {
            current->next = next_consumer->next;
            next_consumer->next = Current_Head;
            Current_Head = next_consumer;
            return Current_Head;
        }

        else
        {

            while (current != nullptr)
            {
                int i = 1;
                while (i < k - 2)
                {
                    if (current->next == nullptr)
                    {
                        return Current_Head;
                    }
                    current = current->next;
                    i++;
                }

                if (current->next == nullptr || current->next->next == nullptr)
                {
                    return Current_Head;
                }

                consumer* kth_node = current->next->next;
                current->next->next = kth_node->next;
                kth_node->next = current->next;
                current->next = kth_node;

                return Current_Head;
            }

        }
    }

}

// 10. Define global integer constants
const int columnWidth = 35;

// 11. Define a function with consumer details
void printConsumer(consumer* cons, int index)
{
    cout << right << setw(columnWidth) << index << ": " << left << cons->description << "(at address: " << cons << ")" << endl; // Extension 1   
    cout << right << setw(columnWidth) << "power consumption" << ": " << left << cons->watt << " W" << endl;
    cout << right << setw(columnWidth) << "power consumption standby" << ": " << left << cons->watt_standby << " W" << endl;
    cout << right << setw(columnWidth) << "annual hours of use" << ": " << left << annualHoursOfUse(cons) << " h" << endl;
    cout << right << setw(columnWidth) << "annual hours of standby" << ": " << left << annualHoursOfStandby(cons) << " h" << endl;
}

// Extension 3. Define a function call input household
void input_household(Household* Ex_house)
{
    char hotwater;

    cout << "How many square meters does the household have: ";
    cin >> Ex_house->squareMeters;
    cout << "how many persons live in this household?";
    cin >> Ex_house->numPersons;
    cout << "is hot water heated using electricity? (y(es) or n(o)) ";
    cin >> hotwater;

    cout << "what is the price for one kWh in EUR? ";
    cin >> Ex_house->powerPrice;
    cin.ignore();
    cout << "who is the power supplier? ";

    getline(cin, Ex_house->powerSupplier);


    if (hotwater == 'y')
        Ex_house->Hot_water = true;
    else
        Ex_house->Hot_water = false;

    Ex_house->consumers = nullptr;
}

// Extension 4. Define a function call copy consumers


Household* copy_consumers(Household* source_house, Household* destination_house)
{
    // Copy all consumers from (source_house to destination_house)

    consumer* temp_house_consumer = source_house->consumers;
    consumer* destination_house_consumer = destination_house->consumers;
    consumer* temp_head = nullptr;
    consumer* temp_tail = nullptr;

    if (temp_house_consumer == nullptr)
    {
        return destination_house;
    }

    while (temp_house_consumer != nullptr)
    {

        consumer* New_consumer = new consumer;
        New_consumer->description = temp_house_consumer->description;
        New_consumer->watt = temp_house_consumer->watt;
        New_consumer->watt_standby = temp_house_consumer->watt_standby;
        New_consumer->hours = temp_house_consumer->hours;
        New_consumer->use = temp_house_consumer->use;
        New_consumer->next = nullptr;


        if (temp_head == nullptr)
        {
            temp_head = New_consumer;
            temp_tail = New_consumer;
        }
        else
        {
            temp_tail->next = New_consumer;
            temp_tail = New_consumer;
        }

        temp_house_consumer = temp_house_consumer->next;
    }
    temp_tail->next = destination_house->consumers;
    destination_house->consumers = temp_head;


    return destination_house;

}



// 12. Funtion print household
void printHousehold(Household* house, int number_of_household) // Extension 2. int number_of_household  
{
    string heater = (house->Hot_water) ? "yes" : "no"; // using Ternary operator

    cout << "H O U S E H O L D  N O  " << " " << number_of_household << "  P O W E R  C O N S U M P T I O N" << endl;
    cout << "---------------------------------------------------------------------" << endl;
    cout << right << setw(columnWidth) << "city"<< ": " << left << house->city <<"(at address: "<<house<<")"<<endl;  // Extension 2. find address for city
    cout << right << setw(columnWidth) << "price for one kWh" << ": " << left << setprecision(2) << fixed << house->powerPrice << " ct/kWh" << endl;
    cout << right << setw(columnWidth) << "power supplier" << ": " << left << house->powerSupplier << endl;
    cout << right << setw(columnWidth) << "square meters" << ": " << left << house->squareMeters << " qm" << endl;
    cout << right << setw(columnWidth) << "persons" << ": " << left << house->numPersons << endl;
    cout << right << setw(columnWidth) << "water heated using electricity" << ": " << left << heater << endl;
    cout << right << setw(columnWidth) << "list of consumers" << ": " << endl;
    cout << "---------------------------------------------------------------------" << endl;

    float powerConsumptionSquareMeters = (house->squareMeters * annualAverageConsumptionPerSquareMeter);
    float powerConsumptionPersons = (house->numPersons * (house->Hot_water ? annual_average_consumption_per_person_with_electric_water_heating : annualAverageConsumptionPerPerson)); // using ternary operator
    float totalConsumption = (powerConsumptionSquareMeters + powerConsumptionPersons);
    float totalCosts = (totalConsumption * (house->powerPrice));
    int index = 1;

    consumer* currentConsumer = house->consumers;

    while (currentConsumer != nullptr)
    {
        totalConsumption += annualKWh(currentConsumer);
        printConsumer(currentConsumer, index);
        cout << right << setw(columnWidth) << "annual consumption" << ": " << left << annualKWh(currentConsumer) << " kWh" << endl;
        cout << right << setw(columnWidth) << "annual costs" << ": " << left << (annualKWh(currentConsumer) * house->powerPrice) << " EUR" << endl;
        currentConsumer = currentConsumer->next;
        index++;
    }

    cout << string(columnWidth * 2, '-') << endl;
    cout << right << setw(columnWidth) << "power consumption square meters" << ": " << left << powerConsumptionSquareMeters << " kWh" << endl;
    cout << right << setw(columnWidth) << "power consumption all persons" << ": " << left << powerConsumptionPersons << " kWh" << endl;
    cout << right << setw(columnWidth) << "total annual power consumption" << ": " << left << totalConsumption << " kWh" << endl;
    cout << right << setw(columnWidth) << "total annual power costs" << ": " << left << (totalConsumption * house->powerPrice) << " EUR" << endl;
}



void readCityInformation(ifstream& ifs, string& taskIdentifier, char separator, int& length, string& city)
{
    // Read a line from the input stream
    string line;
    getline(ifs, line);

    // Extract task identifier
    int i = 0;
    string curr = "";
    while (line[i] != separator)
    {
        curr += line[i];
        i++;
    }
    taskIdentifier = curr;
    curr = "";
    i++;

    // Extract length
    while (line[i] != separator)
    {
        curr += line[i];
        i++;
    }
    i++;
    length = stoi(curr);

    curr = "";

    // Extract city
    while (i < line.length())
    {
        curr += line[i];
        i++;
    }
    city = curr;
}

void readHouseholdInformation(string line, char separator, int startIndex, int& index, string& city, bool& hotWater, int& numberOfPeople, int& squareMeters, float& powerPrice, string& powerSupplier)
{
    // Extract index
    string curr = "";
    int i = startIndex;
    while (line[i] != separator)
    {
        curr += line[i];
        i++;
    }
    i++;
    index = stoi(curr);
    curr = "";

    // Extract city
    while (line[i] != separator)
    {
        curr += line[i];
        i++;
    }
    i++;
    city = curr;
    curr = "";

    // Extract hot water indicator
    while (line[i] != separator)
    {
        curr += line[i];
        i++;
    }
    i++;
    hotWater = ((curr == "true") ? true : false);
    curr = "";

    // Extract number of people
    while (line[i] != separator)
    {
        curr += line[i];
        i++;
    }
    numberOfPeople = stoi(curr);
    curr = "";
    i++;

    while (line[i] != separator)
    {
        curr += line[i];
        i++;
    }
    squareMeters = stoi(curr);
    curr = "";
    i++;

    while (line[i] != separator)
    {
        curr += line[i];
        i++;
    }
    powerPrice = stof(curr);
    curr = "";
    i++;

    while (i < line.length())
    {
        curr += line[i];
        i++;
    }
    powerSupplier = curr;
}

// Convert a string to an enumerated type representing frequency of use
Use readFrequencyOfUse(string curr)
{
    if (curr == "once")
        return Use::once;
    else if (curr == "daily")
        return Use::daily;
    else if (curr == "Monday to Friday")
        return Use::mo_fr;
    else if (curr == "Saturday and Sunday")
        return Use::sa_su;
    else
        return Use::weekly;
}

void readConsumerInformation(string line, char separator, int startIndex, string& description, float& hours, Use& use, float& watt, float& watt_standby)
{
    // Extract description
    string curr = "";
    int i = startIndex;
    while (line[i] != separator)
    {
        curr += line[i];
        i++;
    }
    i++;
    description = curr;
    curr = "";

    // Extract hours
    while (line[i] != separator)
    {
        curr += line[i];
        i++;
    }
    i++;
    hours = stof(curr);
    curr = "";

    // Extract use (frequency of use)
    while (line[i] != separator)
    {
        curr += line[i];
        i++;
    }
    use = readFrequencyOfUse(curr);
    i++;
    curr = "";

    // Extract watt
    while (line[i] != separator)
    {
        curr += line[i];
        i++;
    }
    watt = stof(curr);
    i++;
    curr = "";

    // Extract standby watt
    while (i < line.length())
    {
        curr += line[i];
        i++;
    }
    watt_standby = stof(curr);
}

void readDataFromFile(string fileName, string& taskIdentifier, char separator, Household**& households, int n, int& length, string& city)
{
    // Open the input file
    ifstream ifs(fileName);
    if (!ifs.is_open())
    {
        cout << "Could not open file " << fileName << "!\n";
        return;
    }
    string line;

    cout << "input file " << fileName << " opened...\n";

    // Read city information from the file
    readCityInformation(ifs, taskIdentifier, separator, length, city);

    // Check if the number of households is supported
    if (n < length)
    {
        cout << "file contains up to " << length << " households, but here only " << n << " are supported\n";
    }
    else
    {
        // Read data for each household and consumer from the file
        string curr = "";
        int i = 0;
        Household* house;
        int index;
        while (getline(ifs, line))
        {
            i = 0;
            // Extract the type (household or consumer)
            while (i < line.length() && line[i] != separator)
            {
                curr += line[i];
                i++;
            }
            i++;
            if (curr == "household")
            {
                // Create a new household and read its information
                house = new Household;
                house->consumers = nullptr;
                readHouseholdInformation(line, separator, i, index, house->city, house->Hot_water, house->numPersons, house->squareMeters, house->powerPrice, house->powerSupplier);
                households[index] = house;
            }
            else if (curr == "consumer")
            {
                // Create a new consumer and read its information
                consumer* c = new consumer;
                c->next = nullptr;
                readConsumerInformation(line, separator, i, c->description, c->hours, c->use, c->watt, c->watt_standby);
                addConsumerToHousehold(households[index], c);
            }
            curr = "";
        }
    }

    // Close the input file
    ifs.close();
    cout << "input file " << fileName << " closed...\n";
}



// Write the frequency of use to a file
void writeUseToFile(ofstream& ofs, Use curr)
{
    if (curr == Use::once)
        ofs << "once";
    else if (curr == Use::daily)
        ofs << "daily";
    else if (curr == Use::mo_fr)
        ofs << "Monday to Friday";
    else if (curr == Use::sa_su)
        ofs << "Saturday and Sunday";
    else
        ofs << "weekly";
}

// Write household and consumer data to a file
void writeDataToFile(string fileName, char separator, int n, string city, Household** households)
{
    // Open the output file
    ofstream ofs(fileName);

    cout << "output file " << fileName << " opened...\n";

    // Write header information to the file
    ofs << "A4" << separator << to_string(n) << separator << city << '\n';

    // Iterate through each household
    for (int i = 0; i < n; i++)
    {
        // Check if the household exists
        if (households[i])
        {
            // Write household information to the file
            ofs << "household" << separator << to_string(i) << separator << households[i]->city << separator;
            if (households[i]->Hot_water)
                ofs << "true";
            else
                ofs << "false";
            ofs << separator;
            ofs << to_string(households[i]->numPersons) << separator << to_string(households[i]->squareMeters) << separator;
            ofs << to_string(households[i]->powerPrice) << separator << households[i]->powerSupplier << '\n';

            // Iterate through each consumer in the household
            consumer* curr = households[i]->consumers;
            while (curr)
            {
                // Write consumer information to the file
                ofs << "consumer" << separator << curr->description << separator << curr->hours << separator;
                writeUseToFile(ofs, curr->use);
                ofs << separator << curr->watt << separator << curr->watt_standby << '\n';

                // Move to the next consumer
                curr = curr->next;
            }
        }
    }

    // Close the output file
    cout << "output file " << fileName << " closed...\n";
    ofs.close();
}

// 13. Main Function
int main()
{
    string fileName = "house4.csv";
    string taskIdentifer;
    int length, n;
    string city;
    //float priceKilowattPerHour;

    Household* house = new Household;
    house->consumers = nullptr;

    cout << "CALCULATION OF AVERAGE POWER COSTS FOR A HOUSEHOLD\n";
    cout << "how many households does the house have? ";
    cin >> n;
    Household** number_of_household = new Household * [n];
    for (int i = 0; i < n; i++)
    {
        number_of_household[i] = nullptr;
    }
    cout << "In which city is the household located: ";
    cin >> house->city;
    /*cout << "what is the price for one kWh in EUR?";
    cin >> priceKilowattPerHour;*/

    char separator = ';';


    while (true)
    {

        cout << "q quit" << endl;
        cout << "i input power consumer" << endl;
        cout << "u move up power consumer" << endl;
        cout << "p print household" << endl;
        cout << "a print all households" << endl;
        cout << "n new household" << endl;
        cout << "c copy all cosumers (added to already existing ones)" << endl;
        cout << "r read data from file" << endl;
        cout << "w write data into file\n>> " ;


        char choice;
        cin >> choice;

        if (choice == 'q')
        {
            delete house;
            break;
        }
        else if (choice == 'i')
        {
            consumer* newConsumer, * NewTemp;
            newConsumer = new consumer;
            newConsumer->next = nullptr;
            int num;
            cout << "number of household? ";
            cin >> num;

            cout << "what is the description of the power consumer? ";
            cin.ignore();
            getline(cin, newConsumer->description);
            cout << "how many watt it will have? ";
            cin >> newConsumer->watt;
            cout << "how many watt standby it will have? ";
            cin >> newConsumer->watt_standby;
            input_use("How often is it used?\n(d) daily\n(m) mo_fr\n(o) once\n(s) sa_su\n(w) weekly? ", newConsumer->use);
            cout << "How many hours will it be operating? ";
            cin >> newConsumer->hours;

            addConsumerToHousehold(number_of_household[num], newConsumer);
        }
        else if (choice == 'u')
        {
            int k, num;
            cout << "number of household? ";
            cin >> num;
            cout << "which one? ";
            cin >> k;
            number_of_household[num]->consumers = move_up(number_of_household[num]->consumers, k);
        }
        else if (choice == 'p')
        {
            int number;
            cout << "number of household? ";
            cin >> number;
            printHousehold(number_of_household[number], number);
        }
        else if (choice == 'a')
        {
            for (int i = 0; i < n; i++)
            {
                if (number_of_household[i] != nullptr)
                    printHousehold(number_of_household[i], i);

            }
        }

        else if (choice == 'n')
        {
            int x;
            cout << "number of household? ";
            cin >> x;

            if (number_of_household[x] != nullptr)
            {
                cout << "household already exists" << endl;
            }
            else
            {
                Household* new_household = new Household;
                new_household->city = house->city;
                input_household(new_household);
                number_of_household[x] = new_household;
            }
        }
        else if (choice == 'c')
        {
            int num1, num2;
            cout << "number of household from which to copy consumers? ";
            cin >> num1;
            cout << "nnumber of household to copy to? ";
            cin >> num2;
            if (number_of_household[num1] == nullptr && number_of_household[num2] == nullptr)
            {
                cout << " Both are not exsit ";
            }
            else
            {
                number_of_household[num2] = copy_consumers(number_of_household[num1], number_of_household[num2]);
            }

        }
        else if (choice == 'r')
        {
            string fileName;
            char separator;
            cout << "input file name: ";
            cin >> fileName;

            cout << "input separator character: ";
            cin >> separator;

            ifstream ifs(fileName);

            readDataFromFile(fileName, taskIdentifer, separator, number_of_household, n, length, house->city);
        }
        else if (choice == 'w')
        {
            string fileName;
            char separator;
            cout << "output file name: ";
            cin >> fileName;

            cout << "output separator character: ";
            cin >> separator;

            writeDataToFile(fileName, separator, n, house->city, number_of_household);
        }
        else
        {
            cout << "sorry wrong choice" << endl;
        }
    }

    return 0;
}