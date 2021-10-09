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
              }
              else if (!afterEqual && input[i]!='>' && input[i]!=' ' && input[i]!= '=')
              {
                  tempAttributeKey.push_back(input[i]);
              }

              else if (!afterEqual && input[i]=='=')
                {
                ++i;
                afterEqual = true;
                }
              else if ((afterEqual && input[i]==' ') || (afterEqual && input[i]=='>'))
                {
                afterEqual = false;
                this->attributes.insert({tempAttributeKey,tempAttributeValue});
                tempAttributeKey = "";
                tempAttributeValue = "";
                }
            }
        }
    }
    void setParentTags (const vector<Tag> tags)
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
        for (auto it=parentTags.begin(); it<(parentTags.end()-1);)
        {
            bool itEreased =false;

            for (auto it2=(it+1); it2<parentTags.end();)
            {
                if ((*it2).name[0]=='/' && (*it2).name.substr(1,(*it2).name.size())==(*it).name)
                {
                    parentTags.erase(it2);
                    parentTags.erase(it);
                    itEreased=true;
                    break;
                }
                else
                {
                    ++it2;
                }
            }
            if (!itEreased)
                ++it;
        }
    }

    return parentTags;
}


vector <Tag> updateTagsWithParentTags(const vector<Tag>& tags)
{
    vector <Tag> tags_ = {};
    for (Tag tag : tags)
    {
        vector<Tag> tempParentTags=setParentTags(tags, tag.name);
        tag.setParentTags(tempParentTags);
        tags_.push_back(tag);
    }

    return tags_;
}

class Query
{
public:
    bool complexQuery=false;
    string tagName="";
    string tempAttKey="";
    string::size_type posTilde = string::npos;
    string::size_type posDot =string::npos;
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

    void setProperQuery(const vector <Tag>& tags)
    {
        for (auto tag : tags)
        {
            if (tag.name==this->tagName)
            {
                if (tag.parentTags.size()!=0)
                {
                    for (auto tag_: tag.parentTags)
                    {
                        this->properQuery+=tag_.name + ".";
                    }
                }
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
        setProperQuery(tags_);
    }
};

string getOutput(string input, const vector<Tag>& tags)
{
    string output="";
    bool attFound = false;

    Query query(input,tags);

    for (auto tag : tags)
        {
            if (tag.name==query.tagName)
            {
                for (auto att : tag.attributes)
                {
                    if (att.first==query.tempAttKey)
                    {
                        if (query.complexQuery || tag.parentTags.size()!=0)
                        {
                            if (input.substr(0, query.posDot+1)==query.properQuery)
                            {
                                output = att.second;
                                attFound =true;
                            }
                            else
                                output="Not Found!";
                        }
                        else
                        {
                            output = att.second;
                            attFound =true;
                        }
                    }
                }
                if (!attFound)
                    output="Not Found!";
            }
        }

    if (output=="")
        output="Not Found!";

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
    tags=updateTagsWithParentTags(tags);

    for (string inputForQuery_ : inputsForQueries)
    {
       cout<< getOutput(inputForQuery_, tags)<<endl;
    }

    return 0;
}
