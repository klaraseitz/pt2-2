#include <iostream>
#include <string>
#include <algorithm>
#include <stdio.h>
#include <ctype.h>
#include <vector>

#define ENTRIES 7

std::wstring dataset[ENTRIES][4] = { { L"max.muestermann@bmw.de", L"Max", L"Mustermann", L"SAP" },
    { L"juergen.doellner@hpi.de", L"Jürgen", L"Döllner", L"HPI" },
    { L"soeren.discher@hpi.de", L"Sören", L"Discher", L"HPI" } ,
    { L"daniel.maeller@hpi.de", L"Daniel", L"Mäller", L"HPI" },
    { L"paul.deissler@hertha.de", L"Sebastian", L"Deißler", L"herTha" },
	{ L"mueller.marga@sap.com", L"Marga", L"Müller", L"SAP" },
    { L"h.boss@service.bayer.com", L"Hugo", L"Boss", L"Bayer" }};

//Replaces [from] with [to] in [str]
void ReplaceAll(std::wstring& str, const std::wstring& from, const std::wstring& to) {
    size_t start_pos = 0;
    while ((start_pos = str.find(from, start_pos)) != std::wstring::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length(); // Handles case where 'to' is a substring of 'from'
    }
}

//Splits a given wstring on every appearance of a given char
std::vector<std::wstring> split( std::wstring str, char trenner )
{
    // Rückgabevector definieren
    std::vector<std::wstring> result;
    while ( true )
    {
        int pos = str.find_first_of( trenner );
        // den Teil des Strings vom Anfang bis zur ermittelten Pos holen
        std::wstring cur = str.substr( 0, pos );
        result.push_back( cur );
        //npos = End of string, Trennzeichen war also nicht mehr enthalten
        if ( pos == std::wstring::npos )
            break;
        // Stirng anpassen und ersten Teil entfernen
        str = str.substr( pos + 1 );
    }
    return result;
}

//ToDo 3.1a - implement use std::transform()
void caseConvert(std::wstring& str) {
    std::transform(str.begin(), str.end(), str.begin(), tolower);
}

//ToDo 3.1b - implement replacement for ä, ö, ü, ß with ae,oe,ue,ss
//			- use ReplaceAll. wstrings are declared the same way as normal strings but with an "L" in front
void replace(std::wstring& str) {

    ReplaceAll(str, L"ä", L"ae");
    ReplaceAll(str, L"ö", L"oe");
    ReplaceAll(str, L"ü", L"ue");
    ReplaceAll(str, L"ß", L"ss");
}

//ToDo 3.1c - check if email is correct
bool emailCheck(std::wstring mail, std::wstring firstname, std::wstring name, std::wstring company)
{
    bool namesCorrect = false;
    bool companyCorrect = false;
    bool result = false;
    std::wstring firstCharacterOfFirstname = firstname;
    firstCharacterOfFirstname.resize(1);
       

    std::vector<std::wstring> mailVector = split(mail, '@');

    //check for correctness in the part before the @
    std::wstring firstPartOfMail = mailVector.front();
    std::vector<std::wstring> firstPartOfMailVector = split(firstPartOfMail, '.');
    if ((firstPartOfMailVector.at(0)==firstname && firstPartOfMailVector.at(1) == name) || (firstPartOfMailVector.at(0)==name && firstPartOfMailVector.at(1) == firstname)){
        namesCorrect = true;
    }
    //exceptional case: first name is shortended to the first character
    if(firstPartOfMailVector.at(0)==firstCharacterOfFirstname && firstPartOfMailVector.at(1)==name){
        namesCorrect = true;
    }

    //check for the part behind the @
    std::wstring lastPartOfMail = mailVector.at(1);
    std::vector<std::wstring> lastPartOfMailVector = split(lastPartOfMail, '.');

    for( std::vector<std::wstring>::const_iterator i=lastPartOfMailVector.begin(); i!=lastPartOfMailVector.end(); i++ )
        {
        if(*i == company){
            companyCorrect=true;
        }
    }

    if(namesCorrect && companyCorrect)
        result=true;

    return result;
}

int main(int argc, char * argv[])
{
    for (int i = 0; i < ENTRIES; i++)
    {
        for (int j = 1; j < 4; j++)
        {
            replace(dataset[i][j]);
            caseConvert(dataset[i][j]);
        }

        std::cout << emailCheck(dataset[i][0], dataset[i][1], dataset[i][2], dataset[i][3]) << std::endl;
    }
    
    return 0;
    
}