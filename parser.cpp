#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>
#include <unordered_map>

using namespace std;


class Tag
{
public:
    string name{""};
    unordered_map <string, string> attributes = {};
    vector <Tag> parentTags;

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
        return 0;
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
    void setParentTags (const vector<Tag>& tags)
    {
        this->parentTags = tags;
    }

    void getParentTags()
    {
        for (auto tag : parentTags)
            cout << tag.name <<endl;
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

vector<Tag> setParentTags(const vector <Tag>& tags, const string& tagName)
{
    vector<Tag> parentTags = {};

    for (auto it=tags.begin(); it<tags.end(); ++it)
    {
        if ((*it).name==tagName)
        {
            for (auto it2=tags.begin(); it2<it; ++it2)
            {
                parentTags.push_back(*it2);
            }
            break;
        }
    }

    if (parentTags.begin()!=parentTags.end())
    {
        for (auto it=parentTags.begin(); it<(parentTags.end()-1); ++it)
        {
            for (auto it2=(it+1); it2<parentTags.end(); ++it2)
            {
                //cout << "Parent tag: " << (*it2).name << "has on 0 pos: " << (*it2).name[0] <<endl;
                //cout<< "Its name without / is: "<< (*it2).name.substr(1,(*it2).name.size())<<endl;
                if ((*it2).name[0]=='/' && (*it2).name.substr(1,(*it2).name.size())==(*it).name)
                {
                    parentTags.erase(it2);
                    parentTags.erase(it);
                    break;
                }
            }
        }
    }
    return parentTags;
}


void updateTagsWithParentTags(const vector<Tag>& tags)
{
    for (Tag tag : tags)
    {
        vector<Tag> tempParentTags=setParentTags(tags, tag.name);
        tag.parentTags.insert(tag.parentTags.begin(), tempParentTags.begin(), tempParentTags.end());
        cout<<"Parent tags for "<< tag.name <<":"<< endl;
        tag.getParentTags();
    }
    cout<<"Update done"<<endl;
}

class Query
{
public:
    //string query ="";
    bool complexQuery=false;
    string tagName="";
    //string output="";
    string tempAttKey="";
    string::size_type posTilde = string::npos;
    string::size_type posDot =string::npos;
    //string::size_type prevDot;
    //bool attFound = false;
    vector<Tag> parentTags={};
    //Tag tempParentTag;
    string properQuery = "";

    void setPosDotAndPosTilde(const string& input)
    {
        this->posDot = input.find_last_of('.');
        this->posTilde = input.find('~');
    }

    void setTagNameAndComplexQuery(const string& input)
    {
        if (posDot != std::string::npos)
        {
            for (auto i=(posDot+1); i<posTilde; ++i)
            {
                this->tagName.push_back(input[i]);
            }
            this->complexQuery = true;
        }
        else
        {
            for (auto i=0; i<posTilde; ++i)
            {
                this->tagName.push_back(input[i]);
            }
            this->complexQuery = false;
        }
    }

    void setTempAttKey(const string& input)
    {
        for (auto i=(posTilde+1); i<input.size(); ++i)
        {
            this->tempAttKey.push_back(input[i]);
        }
    }

    /*void setParentTags(const vector <Tag>& tags)
    {
        for (auto it=tags.begin(); it<tags.end(); ++it)
        {
            if ((*it).name==tagName)
            {
                for (auto it2=tags.begin(); it2<it; ++it2)
                {
                    this->parentTags.push_back(*it2);
                }
                break;
            }
        }

        if (parentTags.begin()!=parentTags.end())
        {
            for (auto it=parentTags.begin(); it<(parentTags.end()-1); ++it)
            {
                for (auto it2=(it+1); it2<parentTags.end(); ++it2)
                {
                    //cout << "Parent tag: " << (*it2).name << "has on 0 pos: " << (*it2).name[0] <<endl;
                    //cout<< "Its name without / is: "<< (*it2).name.substr(1,(*it2).name.size())<<endl;
                    if ((*it2).name[0]=='/' && (*it2).name.substr(1,(*it2).name.size())==(*it).name)
                    {
                        this->parentTags.erase(it2);
                        this->parentTags.erase(it);
                        break;
                    }
                }
            }
        }
    }*/

    void setProperQuery()
    {
        if (complexQuery)
        {
            //cout<< tempTag << " is complex tag. Parents: " << endl;
            for (auto tag: parentTags)
            {
                this->properQuery+=tag.name + ".";
                //cout<< tag.name <<" "<< endl;
            }
        }
    }

    Query(const string& input_, const vector <Tag>& tags_)
    {
        auto input = input_;
        auto tags = tags_;
        setPosDotAndPosTilde(input);
        setTagNameAndComplexQuery(input);
        setTempAttKey(input);
        parentTags = setParentTags(tags, tagName);
        setProperQuery();
    }

};

string getOutput(string input, const vector<Tag>& tags)
{
    string output="";
    bool attFound = false;

    Query query(input,tags);
    //query.setup(input, tags);

    for (auto tag : tags)
        {
            if (tag.name==query.tagName)
            {
                /*if (complexTag)
                    cout<<tempTag<<" is complex tag"<< endl;*/
                for (auto att : tag.attributes)
                {
                    //cout<<att.first<<" is found att key"<< endl;
                   //cout<<att.second<<" is found att value"<< endl;
                    if (att.first==query.tempAttKey)
                    {
                        if (query.complexQuery)
                        {
                            //cout<<query.tagName<<" in if"<< endl;
                            if (input.substr(0, query.posDot+1)==query.properQuery)
                            {
                                output = att.second;
                                attFound =true;
                            }
                        }
                        else
                        {
                            //cout<<query.tagName<<" in else"<< endl;
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

    string inputForTag;
    string inputForQuery;
    vector<string> inputsForTags = {};
    vector<string> inputsForQueries = {};
    vector<Tag> tags= {};


    for (int i=0; i<=n; ++i)
    {
        getline(cin, inputForTag);
        inputsForTags.push_back(inputForTag);
    }

    inputsForTags.erase(inputsForTags.begin());

    for (int i=0; i<q; ++i)
    {
        getline(cin, inputForQuery);
        inputsForQueries.push_back(inputForQuery);
    }

    tags = setTags(inputsForTags);
    updateTagsWithParentTags(tags);
    vector <string> dummyNames ={"a","b","c","d","e"};
    //vector <vector <Tag>> dummyParentVectors = {};

    /*for (auto tag : tags)
    {
       cout<< tag.name <<endl;
        for (const auto& [key, value] : tag.attributes)
        {
            cout << key << " = " << value << endl;
        }
    }*/

    for (string inputForQuery_ : inputsForQueries)
    {
       cout<< getOutput(inputForQuery_, tags)<<endl;
    }

    for (auto tag : tags)
    {
        cout<<"Parent tags for "<< tag.name <<":"<< endl;
        for (auto parentTag : tag.parentTags)
            cout<<parentTag.name<<endl;
    }

    for (auto tag : tags)
    {
        cout<<"Parent tags for "<< tag.name <<":"<< endl;
        tag.getParentTags();
    }

    //!!!TO DO setParentTags wywolana raz i vector<Tag> parentTags stworzony raz i wspolny dla wszystkich obiektow,
    //usunac z klas Query i Tag, nizej wzkazowka jak go stworzyc

    for (auto tag : tags)
    {
        vector <Tag> dummyParentVector= setParentTags(tags, tag.name);
        cout<<"Dummy parents for "<<tag.name<<" :"<<endl;
        for(auto dummyParent : dummyParentVector)
        {
            cout<<dummyParent.name<<endl;
        }
    }

    /*cout<< "For dummyVectors:"<<endl;
    for (auto dummyParentVector :dummyParentVectors)
        cout<< tag.name << endl;*/


    return 0;
}
