#include "Classifier.h"
// constructor. create new list and a new object to caluclate the knn algorithm. using abstraction.
Classifier::Classifier() : classifiedVectors(*(new list<tuple<vector<double>, string>>)), calcs(getCalcs())
{
}
// insert every element to the list from the file
void Classifier::getClassifiedVectors(istream &is)
{
    string s;
    while (getline(is, s) && (!s.empty()))
    {
        // getting all the line from the user.
        std::vector<double> vec;
        // split the input into tokens every time in the line that "" is appering the token store the string before the "".
        std::string token;
        // read the data into the stringstream
        std::istringstream iss(s);
        tuple<vector<double>, string> classifiedItem;
        vector<string> tokens;
        // split by any ',' (',' key)
        while (std::getline(iss, token, ','))
        {
            tokens.push_back(token);
        }
        for (int i = 0; i < tokens.size() - 1; i++)
        {
            if (!isValidDouble(tokens[i]))
            {
                throw invalid_argument("Invalid arguments for the Vector");
            }
            // insert the input into the vector.
            get<0>(classifiedItem).push_back(stod(tokens[i]));
        }
        get<1>(classifiedItem) = tokens[tokens.size() - 1];
        this->classifiedVectors.push_back(classifiedItem);
    }
}
// distractor
Classifier::~Classifier()
{
    this->reset();
}

// copy constructor
Classifier::Classifier(const Classifier &other) : classifiedVectors(other.classifiedVectors), calcs(other.calcs)
{
}
// copy assignment operator
Classifier &Classifier::operator=(const Classifier &other)
{
    if (this == &other)
    {
        return *this;
    }
    reset();
    // Use the copy constructor and move assignment operator
    *this = Classifier(other);
    return *this;
}
// move assignment operator
Classifier &Classifier::operator=(Classifier &&other) noexcept
{
    if (this == &other)
    {
        return *this;
    }

    reset();
    *this = Classifier(other);
    return *this;
}

// this function calcualte the distance based on input from the user create new knn based on the string
// the arguments we get are the vector we want to clasify,the K and the desired distance metric
string Classifier::Classify(const vector<double> &Vector, int k, string distanceType)
{
    // get the matching calculator (by distance metric) to classify the vector with the given K.
    return (this->calcs[distanceType])->Classify(Vector, k);
}
// this function check if the input from the user is valid.
bool Classifier::isValidDouble(string s)
{
    if (s.length() == 0)
    {
        return false;
    }
    // we want to get from the user only this characters "0123456789.-" as they represent Double number.
    std::size_t found = s.find_first_not_of("0123456789.-Ee");
    if (found != std::string::npos)
    {
        return false;
    }
    return true;
}
// clean everything we do in the heap.
void Classifier::reset() noexcept
{

    delete (&calcs);
    delete (&classifiedVectors);
}
// create a map that store all the calculators
map<string, CalculatorKnn *> &Classifier::getCalcs()
{
    map<string, CalculatorKnn *> &calcs = *(new map<string, CalculatorKnn *>());
    // add all the calculators to the map.
    calcs["AUC"] = new KnnCalcAuc(this->classifiedVectors);
    calcs["MAN"] = new KnnCalcMan(this->classifiedVectors);
    calcs["CHB"] = new KnnCalcChb(this->classifiedVectors);
    calcs["CAN"] = new KnnCalcCan(this->classifiedVectors);
    calcs["MIN"] = new KnnCalcMin(this->classifiedVectors);
    return calcs;
}
// check if there are any vectors in the classified Vectors list
bool Classifier::isTrained()
{
    return !(this->classifiedVectors.empty());
}