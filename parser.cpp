#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>
#include <unordered_map>

using namespace std;

class Tag{
public:
string name{""};
unordered_map <string, string> attributes = {};
vector <string> parentTags ={};

int setName(string input)
{
    if (input[0]=='<' && input[input.size()-1]=='>')
    {
        for (int i=1; i<input.size(); ++i)
        {
            if (input[i]!='>' && input[i]!=' ')
               this->name.push_back(input[i]);
            else
            {
                return i;
            }
        }
    }
}

void setAttributes(string input, int positionInInput)
{
    int tempI=positionInInput;

    if (input[tempI==' '])
    {
        string tempAttributeKey = "";
        string tempAttributeValue = "";
        bool afterEqual = false;

        for (int i=tempI+1; i<input.size(); ++i)
        {
          if (afterEqual && input[i]!='>' && input[i]!=' ' && input[i]!='"')
          {
              tempAttributeValue.push_back(input[i]);
              //cout<< "<tempAttributeValue=" <<tempAttributeValue <<endl;
          }
          else if (!afterEqual && input[i]!='>' && input[i]!=' ' && input[i]!= '=')
          {
              tempAttributeKey.push_back(input[i]);
              //cout<< "tempAttributeKey="<<tempAttributeKey<<endl;
          }

          else if (!afterEqual && input[i]=='=')
            {
            ++i;
            afterEqual = true;
            //cout<< "AFTER EQUAL"<<endl;
            }
          else if ((afterEqual && input[i]==' ') || (afterEqual && input[i]=='>'))
            {
            afterEqual = false;
            this->attributes.insert({tempAttributeKey,tempAttributeValue});
            //cout<< "tempAttributeKey="<<tempAttributeKey<<endl;
            //cout<< "tempAttributeValue=" <<tempAttributeValue <<endl;
            /*for (const auto& [key, value] : tempTag.attributes)
            {
                cout<< "from constructor" << endl;
                cout << key << " = " << value << endl;
            }*/
            tempAttributeKey = "";
            tempAttributeValue = "";
            }
        }
    }
}
};


vector<Tag> setTags(const vector<string>& inputs)
{
    vector<Tag> tags= {};

    for (auto input: inputs)
    {
        Tag tempTag;
        int positionAfterName = tempTag.setName(input);
        tempTag.setAttributes(input,positionAfterName);
        tags.push_back(tempTag);
    }
    return tags;
}

string getOutput(string query, const vector<Tag>& tags)
{
    string output="";
    string tempTag="";
    string tempAttKey="";
    auto posTilde = query.find('~');
    auto posDot = query.find_last_of('.');
    string::size_type prevDot;
    bool attFound = false;
    bool complexQuery=false;
    vector<Tag> parentTags={};
    Tag tempParentTag;
    string properQuery = "";

        if (posDot != std::string::npos)
        {
            for (auto i=(posDot+1); i<posTilde; ++i)
            {
                tempTag.push_back(query[i]);
            }
            complexQuery = true;
        }
        else
        {
            for (auto i=0; i<posTilde; ++i)
            {
                tempTag.push_back(query[i]);
            }
            complexQuery = false;
        }

        for (auto i=(posTilde+1); i<query.size(); ++i)
        {
            tempAttKey.push_back(query[i]);
        }



        for (auto it=tags.begin(); it<tags.end(); ++it)
        {
            if ((*it).name==tempTag)
            {
                for (auto it2=tags.begin(); it2<it; ++it2)
                {
                    parentTags.push_back(*it2);
                }
                break;
            }
        }


    if (parentTags.begin()!=parentTags.end()){
        for (auto it=parentTags.begin(); it<(parentTags.end()-1); ++it)
        {
            tempParentTag=*it;
            for (auto it2=(it+1); it2<parentTags.end(); ++it2)
            {
                //cout << "Parent tag: " << (*it2).name << "has on 0 pos: " << (*it2).name[0] <<endl;
                //cout<< "Its name without / is: "<< (*it2).name.substr(1,(*it2).name.size())<<endl;
                if ((*it2).name[0]=='/' && (*it2).name.substr(1,(*it2).name.size())==tempParentTag.name)
                {
                    parentTags.erase(it2);
                    parentTags.erase(it);
                    break;
                }

            }
        }
    }

    if (complexQuery)
        {
            cout<< tempTag << " is complex tag. Parents: " << endl;
            for (auto tag: parentTags)
            {
                properQuery+=tag.name + ".";
                cout<< tag.name <<" "<< endl;
            }
        }

    for (auto tag : tags)
        {
            if (tag.name==tempTag)
            {
                /*if (complexTag)
                    cout<<tempTag<<" is complex tag"<< endl;*/
                for (auto att : tag.attributes)
                {
                    //cout<<att.first<<" is found att key"<< endl;
                   //cout<<att.second<<" is found att value"<< endl;
                    if (att.first==tempAttKey)
                    {
                        if (complexQuery)
                        {
                            cout<<tempTag<<" in if"<< endl;
                            if (query.substr(0, posDot+1)==properQuery)
                            {
                                output = att.second;
                                attFound =true;
                            }
                        }
                        else
                        {
                            cout<<tempTag<<" in else"<< endl;
                            output = att.second;
                            attFound =true;
                        }
                        ;
                    }
                }
                if (!attFound)
                    output="Not Found!";
            }
        }

    return output;
}


int main()
{
    string n_;
    string q_;

    cin>> n_ >> q_;

    int n = std::stoi(n_);
    int q = std::stoi(q_);

    string input;
    string query;
    vector<string> inputs = {};
    vector<string> queries = {};
    vector<Tag> tags= {};


    for (int i=0; i<=n; ++i)
    {
        getline(cin, input);
        inputs.push_back(input);
    }

    inputs.erase(inputs.begin());

    for (int i=0; i<q; ++i)
    {
        getline(cin, query);
        queries.push_back(query);
    }

    tags = setTags(inputs);

    /*for (auto tag : tags)
    {
       cout<< tag.name <<endl;
        for (const auto& [key, value] : tag.attributes)
        {
            cout << key << " = " << value << endl;
        }
    }*/

    for (string query_ : queries)
    {
       cout<< getOutput(query_, tags)<<endl;
    }

    return 0;
}
