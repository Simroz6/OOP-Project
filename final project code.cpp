#include <iostream>

#include <fstream>

#include <cstring>

#include <vector>

#include <string>

#include "raylib.h"



using namespace std;



// Screen dimensions

const int screenWidth = 1280;

const int screenHeight = 720;



// Colors

Color BACKGROUND_COLOR = {230, 230, 250, 255};

Color BUTTON_COLOR = {70, 130, 180, 255};

Color BUTTON_HOVER_COLOR = {100, 149, 237, 255};

Color TEXT_COLOR = BLACK;

Color INPUT_BOX_COLOR = LIGHTGRAY;

Color INPUT_BOX_ACTIVE_COLOR = WHITE;



// Font

Font mainFont;



// Input text buffers

char voterNameInput[256] = "\0";

char voterPassInput[256] = "\0";

char voterIdInput[256] = "\0";



char adminNameInput[256] = "\0";

char adminPassInput[256] = "\0";

char adminIdInput[256] = "\0";



char candidateNameInput[256] = "\0";

char candidatePartyInput[256] = "\0";

char candidateIdInput[256] = "\0";



char newVoterNameInput[256] = "\0";

char newVoterPassInput[256] = "\0";

char newVoterIdInput[256] = "\0";

char newVoterVotedInput[256] = "\0";



char removeCandidateIdInput[256] = "\0";

char removeVoterIdInput[256] = "\0";



char selectedCandidateInput[256] = "\0";



// Control variables

bool voterNameEditMode = false;

bool voterPassEditMode = false;

bool voterIdEditMode = false;



bool adminNameEditMode = false;

bool adminPassEditMode = false;

bool adminIdEditMode = false;



bool candidateNameEditMode = false;

bool candidatePartyEditMode = false;

bool candidateIdEditMode = false;



bool newVoterNameEditMode = false;

bool newVoterPassEditMode = false;

bool newVoterIdEditMode = false;

bool newVoterVotedEditMode = false;



bool removeCandidateIdEditMode = false;

bool removeVoterIdEditMode = false;



bool selectedCandidateEditMode = false;



// Application states

enum AppState {

    MAIN_MENU,

    VOTER_LOGIN,

    ADMIN_LOGIN,

    VOTER_PORTAL,

    ADMIN_PORTAL,

    VOTER_CHANGE_PASSWORD,

    VOTER_CAST_VOTE,

    VOTER_VIEW_INSTRUCTIONS,

    ADMIN_ADD_CANDIDATE,

    ADMIN_REMOVE_CANDIDATE,

    ADMIN_ADD_VOTER,

    ADMIN_REMOVE_VOTER,

    ADMIN_VIEW_RESULTS,

    ADMIN_CHECK_DUPLICATES

};



AppState currentState = MAIN_MENU;

AppState previousState = MAIN_MENU;



// Result message

string resultMessage = "";

float resultMessageTimer = 0.0f;

const float RESULT_MESSAGE_DURATION = 3.0f;



// Helper functions

void ShowResultMessage(const string& message) {

    resultMessage = message;

    resultMessageTimer = RESULT_MESSAGE_DURATION;

}



void UpdateResultMessage(float deltaTime) {

    if (resultMessageTimer > 0) {

        resultMessageTimer -= deltaTime;

        if (resultMessageTimer <= 0) {

            resultMessage = "";

        }

    }

}



void DrawResultMessage() {

    if (!resultMessage.empty()) {

        int textWidth = MeasureText(resultMessage.c_str(), 20);

        DrawRectangle(screenWidth/2 - textWidth/2 - 20, 50, textWidth + 40, 40, Fade(BUTTON_COLOR, 0.9f));

        DrawText(resultMessage.c_str(), screenWidth/2 - textWidth/2, 60, 20, WHITE);

    }

}



void ResetInputFields() {

    memset(voterNameInput, 0, sizeof(voterNameInput));

    memset(voterPassInput, 0, sizeof(voterPassInput));

    memset(voterIdInput, 0, sizeof(voterIdInput));



    memset(adminNameInput, 0, sizeof(adminNameInput));

    memset(adminPassInput, 0, sizeof(adminPassInput));

    memset(adminIdInput, 0, sizeof(adminIdInput));



    memset(candidateNameInput, 0, sizeof(candidateNameInput));

    memset(candidatePartyInput, 0, sizeof(candidatePartyInput));

    memset(candidateIdInput, 0, sizeof(candidateIdInput));



    memset(newVoterNameInput, 0, sizeof(newVoterNameInput));

    memset(newVoterPassInput, 0, sizeof(newVoterPassInput));

    memset(newVoterIdInput, 0, sizeof(newVoterIdInput));

    memset(newVoterVotedInput, 0, sizeof(newVoterVotedInput));



    memset(removeCandidateIdInput, 0, sizeof(removeCandidateIdInput));

    memset(removeVoterIdInput, 0, sizeof(removeVoterIdInput));



    memset(selectedCandidateInput, 0, sizeof(selectedCandidateInput));

}



class User {

private:

    string user_name;

    string password;

    int user_id;



public:

    User(string name, string pass, int id) : user_name(name), password(pass), user_id(id) {}



    int getID() {

        return user_id;

    }

    string getUser_name() {

        return user_name;

    }

    string getPassword() {

        return password;

    }



    bool login(string name, string pass, int id) {

        if (name == user_name && pass == password && id == user_id) {

            return true;

        } else {

            return false;

        }

    }



    void change_password(string new_password) {

        password = new_password;

    }



    bool isAuthenticated(string name, string pass, int id) {

        return login(name, pass, id);

    }



    virtual void display() {

        cout << "Name: " << user_name << endl;

        cout << "ID: " << user_id << endl;

    }

};



class Candidate;



class Admin; // Forward declaration



class Voter : public User {

private:

    bool hasVoted;



public:

    Voter(string n, string p, int id, bool voted) : User(n, p, id), hasVoted(voted) {}



    bool getHasVoted() { return hasVoted; }

    void setHasVoted(bool voted) { hasVoted = voted; }



    void cast_vote(Candidate* c[], int count, Admin& admin, const string& candidateName);



    string viewInstructions() {

        return "To cast vote, press the Cast Vote button\nTo login, click the Login button";

    }

};



class Candidate {

private:

    string name;

    string party;

    int ID;

    int voteCount;



public:

    Candidate() {}

    Candidate(string n, string p, int id) : name(n), party(p), ID(id), voteCount(0) {}



    int getID() {

        return ID;

    }



    string getName() const {

        return name;

    }



    string getParty() const {

        return party;

    }



    int getVoteCount() const {

        return voteCount;

    }



    friend void Voter::cast_vote(Candidate* c[], int count, Admin& admin, const string& candidateName);



    void incrementVote() {

        voteCount++;

    }



    string displayDetails() {

        string details = "Candidate Name: " + name + "\n";

        details += "Candidate ID: " + to_string(ID) + "\n";

        details += "Candidate Party: " + party + "\n";

        details += "Candidate Vote Count: " + to_string(voteCount) + "\n";

        return details;

    }



    friend bool operator==(Candidate& c1, Candidate& c2);

};



bool operator==(Candidate& c1, Candidate& c2) {

    return (c1.ID == c2.ID && c1.name == c2.name);

}



class Admin : public User {

public:

    vector<Candidate*> candidates;

    vector<Voter*> voters;



    Admin(string n, string p, int id) : User(n, p, id) {}



    void addCandidate(string n, string p, int id) {

        candidates.push_back(new Candidate(n, p, id));

    }



    void saveCandidatesToFile() {

        ofstream file("candidate_info.txt");

        if (!file) {

            ShowResultMessage("Error opening file to save candidates!");

            return;

        }



        for (auto& candidate : candidates) {

            file << "Name: " << candidate->getName() << endl;

            file << candidate->getID() << endl;

            file << candidate->getVoteCount() << endl;

        }



        file.close();

        ShowResultMessage("Candidates saved successfully.");

    }



    void loadCandidatesFromFile() {

        ifstream file("candidate_info.txt");

        if (!file) {

            ShowResultMessage("Error opening file to load candidates!");

            return;

        }



        for (auto& candidate : candidates) {

            delete candidate;

        }

        candidates.clear();



        string line, name;

        int id, votes;



        while (getline(file, line)) {

            if (line.find("Name: ") == 0) {

                name = line.substr(6);



                if (file >> id >> votes) {

                    file.ignore();

                    Candidate* newCandidate = new Candidate(name, "party", id);

                    for (int i = 0; i < votes; i++) {

                        newCandidate->incrementVote();

                    }

                    candidates.push_back(newCandidate);

                }

            }

        }



        file.close();

        ShowResultMessage("Candidates loaded successfully.");

    }



    void updateCandidate(int id) {

        ifstream inFile("candidate_info.txt");

        if (!inFile) {

            ShowResultMessage("Error opening file for reading!");

            return;

        }



        ofstream tempFile("temp.txt");

        if (!tempFile) {

            ShowResultMessage("Error opening temporary file!");

            return;

        }



        string line, name;

        int candidateID, votes;

        bool found = false;



        while (getline(inFile, line)) {

            if (line.find("Name: ") == 0) {

                name = line.substr(6);

                if (inFile >> candidateID >> votes) {

                    inFile.ignore();

                    if (candidateID == id) {

                        votes++;

                        found = true;

                    }

                    tempFile << "Name: " << name << endl;

                    tempFile << candidateID << endl;

                    tempFile << votes << endl;

                }

            }

        }



        inFile.close();

        tempFile.close();



        remove("candidate_info.txt");

        rename("temp.txt", "candidate_info.txt");



        if (found)

            ShowResultMessage("Candidate vote updated successfully in file.");

        else

            ShowResultMessage("Candidate not found in file.");

    }



    void removeCandidate(int id) {

        for (auto it = candidates.begin(); it != candidates.end(); ++it) {

            if ((*it)->getID() == id) {

                delete *it;

                candidates.erase(it);

                ShowResultMessage("Candidate has been removed");

                return;

            }

        }

        ShowResultMessage("Candidate not found.");

    }



    void addVoter(string n, string p, int id, bool vote) {

        voters.push_back(new Voter(n, p, id, vote));

    }



    void removeVoter(int id) {

        for (auto it = voters.begin(); it != voters.end(); ++it) {

            if ((*it)->getID() == id) {

                delete *it;

                voters.erase(it);

                ShowResultMessage("Voter has been removed");

                return;

            }

        }

        ShowResultMessage("Voter not found.");

    }



    string viewResult() {

        if (candidates.empty()) {

            return "No candidates available.";

        }



        int max = 0;

        Candidate* leadingCandidate = nullptr;



        for (auto& candidate : candidates) {

            if (candidate->getVoteCount() > max) {

                max = candidate->getVoteCount();

                leadingCandidate = candidate;

            }

        }



        if (leadingCandidate == nullptr || max == 0) {

            return "No votes have been cast yet.";

        } else {

            return leadingCandidate->displayDetails();

        }

    }



    string check_duplicate_candidates() {

        for (size_t i = 0; i < candidates.size(); i++) {

            for (size_t j = i + 1; j < candidates.size(); j++) {

                if (*candidates[i] == *candidates[j]) {

                    return "Duplicate candidate found";

                }

            }

        }

        return "There is no duplicate candidate.";

    }



   // Function to get candidate names with dynamic array

string* getCandidateNames(int& count) {

    count = candidates.size();

    string* names = new string[count];

    for (int i = 0; i < count; i++) {

        names[i] = candidates[i]->getName();

    }

    return names;

}



// Function to get voter names with dynamic array

string* getVoterNames(int& count) {

    count = voters.size();

    string* names = new string[count];

    for (int i = 0; i < count; i++) {

        names[i] = voters[i]->getUser_name();

    }

    return names;

}



// Remember to delete[] the returned arrays when done!

};



void Voter::cast_vote(Candidate* c[], int count, Admin& admin, const string& candidateName) {

    bool found = false;

    for (int i = 0; i < count; i++) {

        if (c[i]->getName() == candidateName) {

            if (hasVoted) {

                ShowResultMessage("You have already voted.");

            } else {

                c[i]->incrementVote();

                hasVoted = true;

                ShowResultMessage("Your vote has been casted.");

                admin.updateCandidate(c[i]->getID());

            }

            found = true;

            break;

        }

    }



    if (!found) {

        ShowResultMessage("No such candidate exists.");

    }

}



// Global objects

Voter* currentVoter = nullptr;

Admin admin("Admin", "admin", 1);

vector<string> instructionsText;

vector<string> candidateDetails;

vector<string> voterDetails;

string resultsText;

string duplicatesText;



void InitializeSystem() {

    // Initialize with some default data

    admin.addCandidate("Ali", "PTI", 1);

    admin.addVoter("Voter1", "vote1", 1, false);

    admin.addVoter("Voter2", "vote2", 2, false);



    // Load font

    mainFont = LoadFontEx("fonts/arial.ttf", 32, 0, 0);

    if (mainFont.texture.id == 0) {

        // Fallback to default font if custom font fails to load

        mainFont = GetFontDefault();

    }



    // Initialize instruction text

    string instructions = "Welcome to the E-Voting System\n\n"

                         "Instructions:\n"

                         "1. Voters can login and cast their vote\n"

                         "2. Admins can manage candidates and voters\n"

                         "3. Each voter can vote only once\n"

                         "4. Admins can view results after voting\n\n"

                         "Please select your role to begin.";



    size_t pos = 0;

    string delimiter = "\n";

    while ((pos = instructions.find(delimiter)) != string::npos) {

        instructionsText.push_back(instructions.substr(0, pos));

        instructions.erase(0, pos + delimiter.length());

    }

    instructionsText.push_back(instructions);

}



void UpdateCandidateDetails(Admin& admin) {

    candidateDetails.clear();

    for (auto& candidate : admin.candidates) {

        candidateDetails.push_back(candidate->getName() + " (" + candidate->getParty() + ") - Votes: " + to_string(candidate->getVoteCount()));

    }

}



void UpdateVoterDetails(Admin& admin) {

    voterDetails.clear();

    for (auto& voter : admin.voters) {

        voterDetails.push_back(voter->getUser_name() + " (ID: " + to_string(voter->getID()) + ") - Voted: " + (voter->getHasVoted() ? "Yes" : "No"));

    }

}



void DrawMainMenu() {

    DrawText("E-Voting System", screenWidth/2 - MeasureText("E-Voting System", 40)/2, 100, 40, TEXT_COLOR);



    // Voter Login Button

    Rectangle voterBtn = {screenWidth/2 - 150, 200, 300, 50};

    bool voterBtnHover = CheckCollisionPointRec(GetMousePosition(), voterBtn);

    DrawRectangleRec(voterBtn, voterBtnHover ? BUTTON_HOVER_COLOR : BUTTON_COLOR);

    DrawText("Voter Login", voterBtn.x + voterBtn.width/2 - MeasureText("Voter Login", 20)/2,

             voterBtn.y + voterBtn.height/2 - 10, 20, WHITE);



    // Admin Login Button

    Rectangle adminBtn = {screenWidth/2 - 150, 280, 300, 50};

    bool adminBtnHover = CheckCollisionPointRec(GetMousePosition(), adminBtn);

    DrawRectangleRec(adminBtn, adminBtnHover ? BUTTON_HOVER_COLOR : BUTTON_COLOR);

    DrawText("Admin Login", adminBtn.x + adminBtn.width/2 - MeasureText("Admin Login", 20)/2,

             adminBtn.y + adminBtn.height/2 - 10, 20, WHITE);



    // Exit Button

    Rectangle exitBtn = {screenWidth/2 - 150, 360, 300, 50};

    bool exitBtnHover = CheckCollisionPointRec(GetMousePosition(), exitBtn);

    DrawRectangleRec(exitBtn, exitBtnHover ? RED : MAROON);

    DrawText("Exit", exitBtn.x + exitBtn.width/2 - MeasureText("Exit", 20)/2,

             exitBtn.y + exitBtn.height/2 - 10, 20, WHITE);



    // Handle button clicks

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {

        if (voterBtnHover) {

            currentState = VOTER_LOGIN;

            ResetInputFields();

        } else if (adminBtnHover) {

            currentState = ADMIN_LOGIN;

            ResetInputFields();

        } else if (exitBtnHover) {

            CloseWindow();

        }

    }

}



void DrawVoterLogin() {

    DrawText("Voter Login", screenWidth/2 - MeasureText("Voter Login", 30)/2, 100, 30, TEXT_COLOR);



    // Name Input

    Rectangle nameBox = {screenWidth/2 - 150, 180, 300, 40};

    DrawRectangleRec(nameBox, voterNameEditMode ? INPUT_BOX_ACTIVE_COLOR : INPUT_BOX_COLOR);

    DrawText("Name:", nameBox.x - 100, nameBox.y + 10, 20, TEXT_COLOR);

    DrawText(voterNameInput, nameBox.x + 10, nameBox.y + 10, 20, TEXT_COLOR);



    // Password Input

    Rectangle passBox = {screenWidth/2 - 150, 240, 300, 40};

    DrawRectangleRec(passBox, voterPassEditMode ? INPUT_BOX_ACTIVE_COLOR : INPUT_BOX_COLOR);

    DrawText("Password:", passBox.x - 100, passBox.y + 10, 20, TEXT_COLOR);

    DrawText(voterPassInput, passBox.x + 10, passBox.y + 10, 20, TEXT_COLOR);



    // ID Input

    Rectangle idBox = {screenWidth/2 - 150, 300, 300, 40};

    DrawRectangleRec(idBox, voterIdEditMode ? INPUT_BOX_ACTIVE_COLOR : INPUT_BOX_COLOR);

    DrawText("ID:", idBox.x - 100, idBox.y + 10, 20, TEXT_COLOR);

    DrawText(voterIdInput, idBox.x + 10, idBox.y + 10, 20, TEXT_COLOR);



    // Login Button

    Rectangle loginBtn = {screenWidth/2 - 100, 370, 200, 50};

    bool loginBtnHover = CheckCollisionPointRec(GetMousePosition(), loginBtn);

    DrawRectangleRec(loginBtn, loginBtnHover ? BUTTON_HOVER_COLOR : BUTTON_COLOR);

    DrawText("Login", loginBtn.x + loginBtn.width/2 - MeasureText("Login", 20)/2,

             loginBtn.y + loginBtn.height/2 - 10, 20, WHITE);



    // Back Button

    Rectangle backBtn = {screenWidth/2 - 100, 440, 200, 50};

    bool backBtnHover = CheckCollisionPointRec(GetMousePosition(), backBtn);

    DrawRectangleRec(backBtn, backBtnHover ? GRAY : DARKGRAY);

    DrawText("Back", backBtn.x + backBtn.width/2 - MeasureText("Back", 20)/2,

             backBtn.y + backBtn.height/2 - 10, 20, WHITE);



    // Handle input focus

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {

        voterNameEditMode = CheckCollisionPointRec(GetMousePosition(), nameBox);

        voterPassEditMode = CheckCollisionPointRec(GetMousePosition(), passBox);

        voterIdEditMode = CheckCollisionPointRec(GetMousePosition(), idBox);

    }



    // Handle text input

    if (voterNameEditMode) {

        int key = GetCharPressed();

        while (key > 0) {

            if ((key >= 32) && (key <= 125) && (strlen(voterNameInput) < 255)) {

                strncat(voterNameInput, (char*)&key, 1);

            }

            key = GetCharPressed();

        }



        if (IsKeyPressed(KEY_BACKSPACE) && strlen(voterNameInput) > 0) {

            voterNameInput[strlen(voterNameInput) - 1] = '\0';

        }

    }



    if (voterPassEditMode) {

        int key = GetCharPressed();

        while (key > 0) {

            if ((key >= 32) && (key <= 125) && (strlen(voterPassInput) < 255)) {

                strncat(voterPassInput, (char*)&key, 1);

            }

            key = GetCharPressed();

        }



        if (IsKeyPressed(KEY_BACKSPACE) && strlen(voterPassInput) > 0) {

            voterPassInput[strlen(voterPassInput) - 1] = '\0';

        }

    }



    if (voterIdEditMode) {

        int key = GetCharPressed();

        while (key > 0) {

            if ((key >= 48) && (key <= 57) && (strlen(removeCandidateIdInput) < 255)) {

                strncat(removeCandidateIdInput, (char*)&key, 1);

            }

            key = GetCharPressed();

        }



        if (IsKeyPressed(KEY_BACKSPACE) && strlen(removeCandidateIdInput) > 0) {

            removeCandidateIdInput[strlen(removeCandidateIdInput) - 1] = '\0';

        }

    }



    // Handle button clicks

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {

        if (removeBtnHover) {

            int id = removeCandidateIdInput[0] ? atoi(removeCandidateIdInput) : 0;

            if (id != 0) {

                admin.removeCandidate(id);

                memset(removeCandidateIdInput, 0, sizeof(removeCandidateIdInput));

            } else {

                ShowResultMessage("Please enter a valid ID!");

            }

        } else if (backBtnHover) {

            currentState = previousState;

            memset(removeCandidateIdInput, 0, sizeof(removeCandidateIdInput));

        }

    }

}



void DrawAdminAddVoter() {

    DrawText("Add Voter", screenWidth/2 - MeasureText("Add Voter", 30)/2, 50, 30, TEXT_COLOR);



    // Name Input

    Rectangle nameBox = {screenWidth/2 - 150, 120, 300, 40};

    DrawRectangleRec(nameBox, newVoterNameEditMode ? INPUT_BOX_ACTIVE_COLOR : INPUT_BOX_COLOR);

    DrawText("Name:", nameBox.x - 100, nameBox.y + 10, 20, TEXT_COLOR);

    DrawText(newVoterNameInput, nameBox.x + 10, nameBox.y + 10, 20, TEXT_COLOR);



    // Password Input

    Rectangle passBox = {screenWidth/2 - 150, 180, 300, 40};

    DrawRectangleRec(passBox, newVoterPassEditMode ? INPUT_BOX_ACTIVE_COLOR : INPUT_BOX_COLOR);

    DrawText("Password:", passBox.x - 100, passBox.y + 10, 20, TEXT_COLOR);

    DrawText(newVoterPassInput, passBox.x + 10, passBox.y + 10, 20, TEXT_COLOR);



    // ID Input

    Rectangle idBox = {screenWidth/2 - 150, 240, 300, 40};

    DrawRectangleRec(idBox, newVoterIdEditMode ? INPUT_BOX_ACTIVE_COLOR : INPUT_BOX_COLOR);

    DrawText("ID:", idBox.x - 100, idBox.y + 10, 20, TEXT_COLOR);

    DrawText(newVoterIdInput, idBox.x + 10, idBox.y + 10, 20, TEXT_COLOR);



    // Voted Status Input

    Rectangle votedBox = {screenWidth/2 - 150, 300, 300, 40};

    DrawRectangleRec(votedBox, newVoterVotedEditMode ? INPUT_BOX_ACTIVE_COLOR : INPUT_BOX_COLOR);

    DrawText("Voted (0/1):", votedBox.x - 120, votedBox.y + 10, 20, TEXT_COLOR);

    DrawText(newVoterVotedInput, votedBox.x + 10, votedBox.y + 10, 20, TEXT_COLOR);



    // Add Button

    Rectangle addBtn = {screenWidth/2 - 100, 370, 200, 50};

    bool addBtnHover = CheckCollisionPointRec(GetMousePosition(), addBtn);

    DrawRectangleRec(addBtn, addBtnHover ? BUTTON_HOVER_COLOR : BUTTON_COLOR);

    DrawText("Add", addBtn.x + addBtn.width/2 - MeasureText("Add", 20)/2,

             addBtn.y + addBtn.height/2 - 10, 20, WHITE);



    // Back Button

    Rectangle backBtn = {screenWidth/2 - 100, 440, 200, 50};

    bool backBtnHover = CheckCollisionPointRec(GetMousePosition(), backBtn);

    DrawRectangleRec(backBtn, backBtnHover ? GRAY : DARKGRAY);

    DrawText("Back", backBtn.x + backBtn.width/2 - MeasureText("Back", 20)/2,

             backBtn.y + backBtn.height/2 - 10, 20, WHITE);



    // Handle input focus

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {

        newVoterNameEditMode = CheckCollisionPointRec(GetMousePosition(), nameBox);

        newVoterPassEditMode = CheckCollisionPointRec(GetMousePosition(), passBox);

        newVoterIdEditMode = CheckCollisionPointRec(GetMousePosition(), idBox);

        newVoterVotedEditMode = CheckCollisionPointRec(GetMousePosition(), votedBox);

    }



    // Handle text input

    if (newVoterNameEditMode) {

        int key = GetCharPressed();

        while (key > 0) {

            if ((key >= 32) && (key <= 125) && (strlen(newVoterNameInput) < 255)) {

                strncat(newVoterNameInput, (char*)&key, 1);

            }

            key = GetCharPressed();

        }



        if (IsKeyPressed(KEY_BACKSPACE) && strlen(newVoterNameInput) > 0) {

            newVoterNameInput[strlen(newVoterNameInput) - 1] = '\0';

        }

    }



    if (newVoterPassEditMode) {

        int key = GetCharPressed();

        while (key > 0) {

            if ((key >= 32) && (key <= 125) && (strlen(newVoterPassInput) < 255)) {

                strncat(newVoterPassInput, (char*)&key, 1);

            }

            key = GetCharPressed();

        }



        if (IsKeyPressed(KEY_BACKSPACE) && strlen(newVoterPassInput) > 0) {

            newVoterPassInput[strlen(newVoterPassInput) - 1] = '\0';

        }

    }



    if (newVoterIdEditMode) {

        int key = GetCharPressed();

        while (key > 0) {

            if ((key >= 48) && (key <= 57) && (strlen(newVoterIdInput) < 255)) {

                strncat(newVoterIdInput, (char*)&key, 1);

            }

            key = GetCharPressed();

        }



        if (IsKeyPressed(KEY_BACKSPACE) && strlen(newVoterIdInput) > 0) {

            newVoterIdInput[strlen(newVoterIdInput) - 1] = '\0';

        }

    }



    if (newVoterVotedEditMode) {

        int key = GetCharPressed();

        while (key > 0) {

            if ((key >= 48) && (key <= 49) && (strlen(newVoterVotedInput) < 255)) {

                strncat(newVoterVotedInput, (char*)&key, 1);

            }

            key = GetCharPressed();

        }



        if (IsKeyPressed(KEY_BACKSPACE) && strlen(newVoterVotedInput) > 0) {

            newVoterVotedInput[strlen(newVoterVotedInput) - 1] = '\0';

        }

    }



    // Handle button clicks

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {

        if (addBtnHover) {

            string name = newVoterNameInput;

            string pass = newVoterPassInput;

            int id = newVoterIdInput[0] ? atoi(newVoterIdInput) : 0;

            bool voted = newVoterVotedInput[0] ? (newVoterVotedInput[0] == '1') : false;



            if (!name.empty() && !pass.empty() && id != 0) {

                admin.addVoter(name, pass, id, voted);

                ShowResultMessage("Voter added successfully!");

                memset(newVoterNameInput, 0, sizeof(newVoterNameInput));

                memset(newVoterPassInput, 0, sizeof(newVoterPassInput));

                memset(newVoterIdInput, 0, sizeof(newVoterIdInput));

                memset(newVoterVotedInput, 0, sizeof(newVoterVotedInput));

            } else {

                ShowResultMessage("Please fill all fields correctly!");

            }

        } else if (backBtnHover) {

            currentState = previousState;

            memset(newVoterNameInput, 0, sizeof(newVoterNameInput));

            memset(newVoterPassInput, 0, sizeof(newVoterPassInput));

            memset(newVoterIdInput, 0, sizeof(newVoterIdInput));

            memset(newVoterVotedInput, 0, sizeof(newVoterVotedInput));

        }

    }

}



void DrawAdminRemoveVoter() {

    DrawText("Remove Voter", screenWidth/2 - MeasureText("Remove Voter", 30)/2, 50, 30, TEXT_COLOR);



    // Display current voters

    int yPos = 100;

    for (size_t i = 0; i < admin.voters.size(); i++) {

        string voterText = to_string(admin.voters[i]->getID()) + " - " + admin.voters[i]->getUser_name();

        DrawText(voterText.c_str(), screenWidth/2 - MeasureText(voterText.c_str(), 20)/2, yPos, 20, TEXT_COLOR);

        yPos += 30;

    }



    // ID Input

    Rectangle idBox = {screenWidth/2 - 150, yPos + 20, 300, 40};

    DrawRectangleRec(idBox, removeVoterIdEditMode ? INPUT_BOX_ACTIVE_COLOR : INPUT_BOX_COLOR);

    DrawText("Voter ID:", idBox.x - 120, idBox.y + 10, 20, TEXT_COLOR);

    DrawText(removeVoterIdInput, idBox.x + 10, idBox.y + 10, 20, TEXT_COLOR);



    // Remove Button

    Rectangle removeBtn = {screenWidth/2 - 100, yPos + 80, 200, 50};

    bool removeBtnHover = CheckCollisionPointRec(GetMousePosition(), removeBtn);

    DrawRectangleRec(removeBtn, removeBtnHover ? BUTTON_HOVER_COLOR : BUTTON_COLOR);

    DrawText("Remove", removeBtn.x + removeBtn.width/2 - MeasureText("Remove", 20)/2,

             removeBtn.y + removeBtn.height/2 - 10, 20, WHITE);



    // Back Button

    Rectangle backBtn = {screenWidth/2 - 100, yPos + 150, 200, 50};

    bool backBtnHover = CheckCollisionPointRec(GetMousePosition(), backBtn);

    DrawRectangleRec(backBtn, backBtnHover ? GRAY : DARKGRAY);

    DrawText("Back", backBtn.x + backBtn.width/2 - MeasureText("Back", 20)/2,

             backBtn.y + backBtn.height/2 - 10, 20, WHITE);



    // Handle input focus

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {

        removeVoterIdEditMode = CheckCollisionPointRec(GetMousePosition(), idBox);

    }



    // Handle text input

    if (removeVoterIdEditMode) {

        int key = GetCharPressed();

        while (key > 0) {

            if ((key >= 48) && (key <= 57) && (strlen(removeVoterIdInput) < 255)) {

                strncat(removeVoterIdInput, (char*)&key, 1);

            }

            key = GetCharPressed();

        }



        if (IsKeyPressed(KEY_BACKSPACE) && strlen(removeVoterIdInput) > 0) {

            removeVoterIdInput[strlen(removeVoterIdInput) - 1] = '\0';

        }

    }



    // Handle button clicks

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {

        if (removeBtnHover) {

            int id = removeVoterIdInput[0] ? atoi(removeVoterIdInput) : 0;

            if (id != 0) {

                admin.removeVoter(id);

                memset(removeVoterIdInput, 0, sizeof(removeVoterIdInput));

            } else {

                ShowResultMessage("Please enter a valid ID!");

            }

        } else if (backBtnHover) {

            currentState = previousState;

            memset(removeVoterIdInput, 0, sizeof(removeVoterIdInput));

        }

    }

}



void DrawAdminViewResults() {

    DrawText("Voting Results", screenWidth/2 - MeasureText("Voting Results", 30)/2, 50, 30, TEXT_COLOR);



    // Display results

    int yPos = 100;

    vector<string> resultLines;

    size_t pos = 0;

    string delimiter = "\n";

    string tempResults = resultsText;



    while ((pos = tempResults.find(delimiter)) != string::npos) {

        resultLines.push_back(tempResults.substr(0, pos));

        tempResults.erase(0, pos + delimiter.length());

    }

    resultLines.push_back(tempResults);



    for (const auto& line : resultLines) {

        DrawText(line.c_str(), screenWidth/2 - MeasureText(line.c_str(), 20)/2, yPos, 20, TEXT_COLOR);

        yPos += 30;

    }



    // Back Button

    Rectangle backBtn = {screenWidth/2 - 100, yPos + 50, 200, 50};

    bool backBtnHover = CheckCollisionPointRec(GetMousePosition(), backBtn);

    DrawRectangleRec(backBtn, backBtnHover ? GRAY : DARKGRAY);

    DrawText("Back", backBtn.x + backBtn.width/2 - MeasureText("Back", 20)/2,

             backBtn.y + backBtn.height/2 - 10, 20, WHITE);



    // Handle button clicks

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {

        if (backBtnHover) {

            currentState = previousState;

        }

    }

}



void DrawAdminCheckDuplicates() {

    DrawText("Check Duplicate Candidates", screenWidth/2 - MeasureText("Check Duplicate Candidates", 30)/2, 50, 30, TEXT_COLOR);



    // Display duplicate check result

    DrawText(duplicatesText.c_str(), screenWidth/2 - MeasureText(duplicatesText.c_str(), 20)/2, 120, 20, TEXT_COLOR);



    // Back Button

    Rectangle backBtn = {screenWidth/2 - 100, 200, 200, 50};

    bool backBtnHover = CheckCollisionPointRec(GetMousePosition(), backBtn);

    DrawRectangleRec(backBtn, backBtnHover ? GRAY : DARKGRAY);

    DrawText("Back", backBtn.x + backBtn.width/2 - MeasureText("Back", 20)/2,

             backBtn.y + backBtn.height/2 - 10, 20, WHITE);



    // Handle button clicks

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {

        if (backBtnHover) {

            currentState = previousState;

        }

    }

}



int main() {

    // Initialize window

    InitWindow(screenWidth, screenHeight, "E-Voting System");

    SetTargetFPS(60);



    InitializeSystem();



    // Main game loop

    while (!WindowShouldClose()) {

        float deltaTime = GetFrameTime();

        UpdateResultMessage(deltaTime);



        BeginDrawing();

        ClearBackground(BACKGROUND_COLOR);



        // Draw based on current state

        switch (currentState) {

            case MAIN_MENU:

                DrawMainMenu();

                break;

            case VOTER_LOGIN:

                DrawVoterLogin();

                break;

            case ADMIN_LOGIN:

                DrawAdminLogin();

                break;

            case VOTER_PORTAL:

                DrawVoterPortal();

                break;

            case VOTER_CHANGE_PASSWORD:

                DrawVoterChangePassword();

                break;

            case VOTER_CAST_VOTE:

                DrawVoterCastVote();

                break;

            case VOTER_VIEW_INSTRUCTIONS:

                DrawVoterViewInstructions();

                break;

            case ADMIN_PORTAL:

                DrawAdminPortal();

                break;

            case ADMIN_ADD_CANDIDATE:

                DrawAdminAddCandidate();

                break;

            case ADMIN_REMOVE_CANDIDATE:

                DrawAdminRemoveCandidate();

                break;

            case ADMIN_ADD_VOTER:

                DrawAdminAddVoter();

                break;

            case ADMIN_REMOVE_VOTER:

                DrawAdminRemoveVoter();

                break;

            case ADMIN_VIEW_RESULTS:

                DrawAdminViewResults();

                break;

            case ADMIN_CHECK_DUPLICATES:

                DrawAdminCheckDuplicates();

                break;

        }



        // Always draw result message if there is one

        DrawResultMessage();



        EndDrawing();

    }



    // Cleanup

    CloseWindow();



    return 0;

}

