#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>

using namespace std;


int inputInteger(const string& prompt) {
	string input;
	int value;
	size_t processed_chars;
	while (true) {
		cout << prompt;
		getline(cin, input);
		try {
			value = stoi(input, &processed_chars);
			if (processed_chars == input.length() && value >= 0) {
				return value;
			}
			else {
				cout << "Enter correct number!" << endl;
			}
		}
		catch (...) {
			cout << "Enter correct number!" << endl;
		}
	}
}

double inputDouble(const string& prompt) {
	string input;
	double value;
	size_t processed_chars;
	while (true) {
		cout << prompt;
		getline(cin, input);
		try {
			value = stod(input, &processed_chars);
			if (processed_chars == input.length() && value > 0) {
				return value;
			}
			else {
				cout << "Enter correct number!" << endl;
			}
		}
		catch (...) {
			cout << "Enter correct number!" << endl;
		}
	}
}

bool isInRepair(const string& prompt) {
	string input;
	while (true) {
		cout << prompt;
		getline(cin, input);
		if (input == "Y" || input == "y") {
			return true;
		}
		else if (input == "N" || input == "n") {
			return false;
		}
		else {
			cout << "Enter Y/N" << endl;
		}
	}
}


struct Pipe {
	static int nextID;
	int id;
	double length;
	double diameter;
	bool inRepair;

	Pipe(double length, double diameter, bool inRepair)
		: id(nextID++), length(length), diameter(diameter), inRepair(inRepair) {}

	static Pipe createPipe() {
		double length = inputDouble("Enter length: ");
		double diameter = inputDouble("Enter diameter: ");
		bool inRepair = isInRepair("Is the pipe being repaired? (Y/N):  ");
		return Pipe(length, diameter, inRepair);
	}

	void display() const {
		cout << "Pipe ID: " << id << ", Length: " << length << ", Diameter: " << diameter
			<< ", In Repair: " << (inRepair ? "Yes" : "No") << endl;
	}

	void edit() {
		cout << "Editing a pipe (ID " << id << ")\n";
		length = inputDouble("Enter new length: ");
		diameter = inputDouble("Enter new diameter: ");
		inRepair = isInRepair("Is the pipe being repaired? (Y/N): ");
	}
};

struct CS {
	string name;
	double stationsCount;
	double workingSts;
	double effectivity;
	

	CS(string name, double stationsCount, double workingSts, double effectivity)
		: name(name), stationsCount(stationsCount), workingSts(workingSts), effectivity(effectivity) {}

	static CS createCS() {
		string name;
		cout << "Enter name of CS: ";
		getline(cin, name);

		double stationsCount;
		while (true) {
			stationsCount = inputDouble("Enter the number of stations: ");
			if (stationsCount > 0) {
				break;
			}
			else {
				cout << "The number of stations must be greater than 0. Please try again." << endl;
			}
		}
		double workingSts;
		while (true) {
			workingSts = inputDouble("Enter the number of working stations: ");
			if (workingSts >= 0 && workingSts <= stationsCount) {
				break;
			}
			else {
				cout << "The number of working stations must be between 0 and " << stationsCount << ". Please try again." << endl;
			}
		}
		double effectivity;
		while (true) {
			effectivity = inputDouble("Enter the CS effectivity (0-100): ");
			if (effectivity >= 0 && effectivity <= 100) {
				break;
			}
			else {
				cout << "Effectivity must be between 0 and 100. Please try again." << endl;
			}
		}
		return CS(name, stationsCount, workingSts, effectivity);
	}

	void display() const {
		cout << "CS name: " << name << ", the number of stations: " << stationsCount << ", the number of working stations: " << workingSts
			<< ", Effectivity: " << effectivity << endl;
	}
	void edit() {
		cout << "Editing a CS (Name " << name << ")" << endl;
		while (true) {
			stationsCount = inputDouble("Enter the number of stations: ");
			if (stationsCount > 0) {
				break;
			}
			else {
				cout << "The number of stations must be greater than 0. Please try again." << endl;
			}
		}
		while (true) {
			workingSts = inputDouble("Enter the new number of stations: ");
			if (workingSts >= 0 && workingSts <= stationsCount) {
				break;
			}
			else {
				cout << "The number of stations must be greater than 0. Please try again." << endl;
			}
		}
		while (true) {
			effectivity = inputDouble("Enter the new effectivity: ");
			if (0 <= effectivity && effectivity <= 100) {
				break;
			}
			else {
				cout << "Effectivity must be between 0 and 100. Please try again." << endl;
			}
		}
	}
};


int Pipe::nextID = 1;


void displayMenu() {
	cout << endl << "===Menu===" << endl
		<< "1.Add pipe" << endl
		<< "2.Add CS" << endl
		<< "3.View all objects" << endl
		<< "4.Edit pipe" << endl
		<< "5.Edit CS" << endl
		<< "6.Save" << endl
		<< "7.Load" << endl
		<< "0.Exit" << endl;
}


void editPipe(vector<Pipe>& pipes) {
	if (pipes.empty()) {
		cout << "No pipes available for editing." << endl;
		return;
	}

	bool found = false;
	while (!found) {
		int pipeID = inputInteger("Enter the ID of the pipe to edit: ");

		for (auto& pipe : pipes) {
			if (pipe.id == pipeID) {
				pipe.edit();
				found = true;
				cout << "Pipe with ID " << pipeID << " has been successfully updated." << endl;
				break;
			}
		}
		if (!found) {
			cout << "Pipe with ID " << pipeID << " not found." << endl;
		}
	}
}


void editCS(vector<CS>& stations) {
	if (stations.empty()) {
		cout << "No CS available for editing." << endl;
		return;
	}

	string csName;
	bool found = false;
	while (!found) {
		cout << "Enter the name of the CS to edit: ";
		getline(cin, csName);
		for (auto& station : stations) {
			if (station.name == csName) {
				station.edit();
				found = true;
				cout << "CS with name " << station.name << " has been succesfully update." << endl;
				break;
			}
		}
		if (!found) {
			cout << "CS with name " << csName << " not found." << endl << endl;
		}
	}
}


void saveToFile(const vector<Pipe>& pipes, const vector<CS>& stations) {
	ofstream outFile("output.txt");
	
	if (!pipes.empty()) {
		for (const auto& pipe : pipes) {
			outFile << "Pipe ID: " << pipe.id << ", Length: " << pipe.length << ", Diameter: " << pipe.diameter
				<< ", In Repair: " << (pipe.inRepair ? "Yes" : "No") << endl;
		}
	}

	if (!stations.empty()) {
		for (const auto& station : stations) {
			outFile << "CS name: " << station.name << ", the number of stations: " << station.stationsCount << ", the number of working stations: " << station.workingSts
				<< ", Effectivity: " << station.effectivity << endl;
		}
	}
	outFile.close();
	cout << "Data succesfully saved to output.txt" << endl;
}


void loadFromFile(vector<Pipe>& pipes, vector<CS>& stations) {
	ifstream inFile("output.txt");

	if (!inFile.is_open()) {
		cout << "Error: Unable to open file for loading" << endl;
		return;
	}

	string line;
	pipes.clear();
	stations.clear();

	while (getline(inFile, line)) {
		if (line.empty()) continue;

		if (line.find("Pipe ID:") != string::npos) {
			int id;
			double length, diameter;
			bool inRepair;

			size_t idPos = line.find("Pipe ID:") + 8;
			size_t lengthPos = line.find("Length:") + 8;
			size_t diameterPos = line.find("Diameter:") + 9;
			size_t repairPos = line.find("In Repair:") + 11;

			id = stoi(line.substr(idPos, line.find(",", idPos) - idPos));
			length = stod(line.substr(lengthPos, line.find(",", lengthPos) - lengthPos));
			diameter = stod(line.substr(diameterPos, line.find(",", diameterPos) - diameterPos));
			string inRepairStr = line.substr(repairPos);

			inRepair = (inRepairStr == "Yes");

			Pipe pipe(length, diameter, inRepair);
			pipe.id = id;
			pipes.push_back(pipe);
		}
		else if (line.find("CS name:") != string::npos) {
			string name;
			double stationsCount, workingSts, effectivity;

			size_t namePos = line.find("CS name:") + 9;
			size_t stationsCountPos = line.find("the number of stations:") + 24;
			size_t workingStsPos = line.find("the number of working stations:") + 32;
			size_t effectivityPos = line.find("Effectivity:") + 13;

			name = line.substr(namePos, line.find(",", namePos) - namePos);
			stationsCount = stod(line.substr(stationsCountPos, line.find(",", stationsCountPos) - stationsCountPos));
			workingSts = stod(line.substr(workingStsPos, line.find(",", workingStsPos) - workingStsPos));
			effectivity = stod(line.substr(effectivityPos));

			if (name.empty() || stationsCount < 0 || workingSts < 0 || effectivity < 0) {
				cout << "Error loading data: Invalid values found in line: " << line << endl;
				continue;
			}

			CS station(name, stationsCount, workingSts, effectivity);
			stations.push_back(station);
		}
	}

	inFile.close();
	cout << "Data successfully loaded from output.txt" << endl;
}


int main() {
	vector<Pipe> pipes;
	vector<CS> stations;
	bool flag = true;

	while (flag) {
		displayMenu();
		int choice = inputInteger("Your choice: ");
		switch (choice)
		{
		case 1: {
			  Pipe newPipe = Pipe::createPipe();
			  pipes.push_back(newPipe);
			  break;
		}
		case 2: {
			CS newCS = CS::createCS();
			stations.push_back(newCS);
			break;
		}
		case 3: {
			for (const auto& pipe : pipes) {
				pipe.display();
			}
			cout << endl;
			for (const auto& station : stations) {
				station.display();
			}
			break;
		}
		case 4:
			editPipe(pipes);
			break;
		case 5:
			editCS(stations);
			break;
		case 6:
			saveToFile(pipes, stations);
			break;
		case 7:
			loadFromFile(pipes, stations);
			break;
		case 0:
			flag = false;
			break;
		default:
			cout << "Enter number from 0 to 7" << endl;
			break;
		}
	}
}
