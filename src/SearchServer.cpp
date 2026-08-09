#include "SearchServer.h"

#include <sstream>
#include <set>
#include <map>
#include <algorithm>

using namespace std;

SearchServer::SearchServer(InvertedIndex& idx, size_t max_responses)
    : index(idx), max_responses(max_responses)
{
}

vector<vector<RelativeIndex>> SearchServer::search(
    const vector<string>& queries_input)
{
    vector<vector<RelativeIndex>> result;

    for (const auto& query : queries_input)
    {
        // Разбиваем запрос на уникальные слова
        stringstream ss(query);
        set<string> uniqueWords;

        string word;

        while (ss >> word)
        {
            uniqueWords.insert(word);
        }

        if (uniqueWords.empty())
        {
            result.push_back({});
            continue;
        }

        // Считаем абсолютную релевантность каждого документа

        map<size_t, size_t> relevance;

        for (const auto& currentWord : uniqueWords)
        {
            vector<Entry> entries =
                index.GetWordCount(currentWord);

            for (const auto& entry : entries)
            {
                relevance[entry.doc_id] += entry.count;
            }
        }

        // Если ни одного совпадения нет
        if (relevance.empty())
        {
            result.push_back({});
            continue;
        }

        // Находим максимальную релевантность

        size_t maxRelevance = 0;

        for (const auto& item : relevance)
        {
            if (item.second > maxRelevance)
            {
                maxRelevance = item.second;
            }
        }

        // Переводим абсолютную релевантность
        // в относительную

        vector<RelativeIndex> queryResult;

        for (const auto& item : relevance)
        {
            RelativeIndex relativeIndex;

            relativeIndex.doc_id = item.first;

            relativeIndex.rank =
                static_cast<float>(item.second) /
                maxRelevance;

            queryResult.push_back(relativeIndex);
        }

        // Сортируем:
        // сначала rank по убыванию,
        // затем doc_id по возрастанию

        sort(
            queryResult.begin(),
            queryResult.end(),
            [](const RelativeIndex& a, const RelativeIndex& b)
            {
                if (a.rank == b.rank)
                {
                    return a.doc_id < b.doc_id;
                }

                return a.rank > b.rank;
            }
        );

        // Оставляем максимум 5 результатов

        if (queryResult.size() > max_responses)
        {
            queryResult.resize(max_responses);
        }

        result.push_back(queryResult);
    }

    return result;
}