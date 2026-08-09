#include "ConverterJSON.h"

#include <fstream>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <stdexcept>

#include <nlohmann/json.hpp>

using json = nlohmann::json;
using namespace std;

vector<string> ConverterJSON::GetTextDocuments()
{
    vector<string> documents;

    ifstream configFile("config.json");

    if (!configFile)
    {
        throw runtime_error("config file is missing");
    }

    json configData;
    configFile >> configData;

    if (!configData.contains("config"))
    {
        throw runtime_error("config file is empty");
    }

    if (!configData.contains("files"))
    {
        return documents;
    }

    for (const auto& file : configData["files"])
    {
        ifstream document(file.get<string>());

        if (!document)
        {
            cerr << "Cannot open file: "
                 << file.get<string>()
                 << endl;

            continue;
        }

        stringstream buffer;
        buffer << document.rdbuf();

        documents.push_back(buffer.str());
    }

    return documents;
}

int ConverterJSON::GetResponseLimit()
{
    ifstream configFile("config.json");

    if (!configFile)
    {
        throw runtime_error("config file is missing");
    }

    json configData;
    configFile >> configData;

    if (!configData.contains("config"))
    {
        throw runtime_error("config file is empty");
    }

    if (!configData["config"].contains("max_responses"))
    {
        throw runtime_error("max_responses is missing");
    }

    return configData["config"]["max_responses"].get<int>();
}

vector<string> ConverterJSON::GetRequests()
{
    vector<string> requests;

    ifstream requestFile("requests.json");

    if (!requestFile)
    {
        throw runtime_error("requests file is missing");
    }

    json requestData;
    requestFile >> requestData;

    if (!requestData.contains("requests"))
    {
        throw runtime_error("requests file is empty");
    }

    for (const auto& request : requestData["requests"])
    {
        requests.push_back(request.get<string>());
    }

    return requests;
}

void ConverterJSON::putAnswers(
    const vector<vector<RelativeIndex>>& answers)
{
    json result;

    result["answers"] = json::object();

    int maxResponses = GetResponseLimit();

    for (size_t i = 0; i < answers.size(); i++)
    {
        stringstream requestName;

        requestName << "request"
                    << setw(3)
                    << setfill('0')
                    << i + 1;

        if (answers[i].empty())
        {
            result["answers"][requestName.str()]["result"] = false;

            continue;
        }

        result["answers"][requestName.str()]["result"] = true;

        size_t limit = min(
            static_cast<size_t>(maxResponses),
            answers[i].size()
        );

        for (size_t j = 0; j < limit; j++)
        {
            result["answers"][requestName.str()]
                ["relevance"]
                .push_back(
                {
                    {"docid", answers[i][j].doc_id},
                    {"rank", answers[i][j].rank}
                });
        }
    }

    ofstream output("answers.json");

    if (!output)
    {
        throw runtime_error(
            "cannot create answers.json"
        );
    }

    output << setw(4) << result;
}