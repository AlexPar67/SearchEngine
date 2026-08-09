#include "ConverterJSON.h"
#include "InvertedIndex.h"
#include "SearchServer.h"

#include <iostream>

using namespace std;

int main()
{
    try
    {
        ConverterJSON converter;

        vector<string> documents =
            converter.GetTextDocuments();

        InvertedIndex index;
        index.UpdateDocumentBase(documents);

        size_t maxResponses = converter.GetResponseLimit();

        SearchServer searchServer(index, maxResponses);

        vector<string> requests =
            converter.GetRequests();

        vector<vector<RelativeIndex>> answers =
            searchServer.search(requests);

        converter.putAnswers(answers);

        cout << "Search completed successfully." << endl;
    }
    catch (const exception& error)
    {
        cerr << "Error: " << error.what() << endl;
        return 1;
    }

    return 0;
}