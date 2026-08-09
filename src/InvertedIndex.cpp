#include "InvertedIndex.h"

#include <sstream>
#include <thread>
#include <algorithm>

using namespace std;

void InvertedIndex::UpdateDocumentBase(
    const vector<string>& input_docs)
{
    docs = input_docs;
    freq_dictionary.clear();

    // Если документов нет — нечего индексировать
    if (docs.empty())
    {
        return;
    }

    // Определяем количество потоков
    const size_t hardware_threads =
        thread::hardware_concurrency();

    const size_t thread_count =
        hardware_threads == 0
            ? 2
            : min(hardware_threads, docs.size());

    // Локальный индекс для каждого потока
    vector<map<string, vector<Entry>>> local_dictionaries(
        thread_count);

    vector<thread> threads;
    threads.reserve(thread_count);

    // Распределяем документы между потоками
    const size_t documents_per_thread =
        (docs.size() + thread_count - 1) / thread_count;

    for (size_t thread_id = 0;
         thread_id < thread_count;
         thread_id++)
    {
        const size_t begin =
            thread_id * documents_per_thread;

        const size_t end =
            min(begin + documents_per_thread, docs.size());

        if (begin >= end)
        {
            continue;
        }

        threads.emplace_back(
            [this, &local_dictionaries, thread_id, begin, end]()
            {
                auto& local_dictionary =
                    local_dictionaries[thread_id];

                for (size_t doc_id = begin;
                     doc_id < end;
                     doc_id++)
                {
                    const string& document = docs[doc_id];

                    stringstream ss(document);
                    string word;

                    while (ss >> word)
                    {
                        auto it =
                            local_dictionary.find(word);

                        if (it == local_dictionary.end())
                        {
                            local_dictionary[word].push_back(
                                {doc_id, 1});
                        }
                        else
                        {
                            if (it->second.back().doc_id == doc_id)
                            {
                                it->second.back().count++;
                            }
                            else
                            {
                                it->second.push_back(
                                    {doc_id, 1});
                            }
                        }
                    }
                }
            });
    }

    // Ждём завершения всех потоков
    for (auto& thread : threads)
    {
        thread.join();
    }

    // Объединяем локальные индексы
    for (const auto& local_dictionary :
         local_dictionaries)
    {
        for (const auto& [word, entries] :
             local_dictionary)
        {
            auto& destination =
                freq_dictionary[word];

            destination.insert(
                destination.end(),
                entries.begin(),
                entries.end());
        }
    }
}

vector<Entry> InvertedIndex::GetWordCount(
    const string& word)
{
    auto it = freq_dictionary.find(word);

    if (it != freq_dictionary.end())
    {
        return it->second;
    }

    return {};
}