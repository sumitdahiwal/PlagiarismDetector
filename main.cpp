#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <cctype>
#include <sstream>
#include <functional>
#include <unordered_map>
#include <unordered_set>
#include <set>
#include <map>
#include <queue>
#include <iomanip>
#include <chrono>
#include <io.h>

using namespace std;


// ============================================
// CONFIGURATION
// ============================================

const int SHINGLE_SIZE = 3;

const int TOP_K = 5;

const double PLAGIARISM_THRESHOLD = 30.0;

const double WORD_WEIGHT = 0.40;

const double SHINGLE_WEIGHT = 0.60;

const int MIN_IMPORTANT_WORD_LENGTH = 4;

const int MIN_SHARED_WORDS = 2;


// ============================================
// RESULT STRUCTURE
// ============================================

struct SimilarityResult {

    int doc1;

    int doc2;

    double wordSimilarity;

    double shingleSimilarity;

    double finalScore;

    int sharedWords;

    int sharedShingles;
};


// ============================================
// READ DOCUMENT
// ============================================

string readDocument(
    const string& filename
) {

    ifstream file(
        filename.c_str()
    );

    if (!file.is_open()) {

        return "";
    }

    string content;

    string line;

    while (
        getline(file, line)
    ) {

        content += line;

        content += " ";
    }

    file.close();

    return content;
}


// ============================================
// PREPROCESS TEXT
// ============================================

string preprocessText(
    string text
) {

    for (
        size_t i = 0;
        i < text.length();
        i++
    ) {

        unsigned char c =
            static_cast<unsigned char>(
                text[i]
            );

        if (
            ispunct(c)
        ) {

            text[i] = ' ';

        } else {

            text[i] =
                static_cast<char>(
                    tolower(c)
                );
        }
    }

    return text;
}


// ============================================
// GET WORDS
// ============================================

vector<string> getWords(
    const string& text
) {

    vector<string> words;

    stringstream ss(text);

    string word;

    while (
        ss >> word
    ) {

        words.push_back(
            word
        );
    }

    return words;
}


// ============================================
// CREATE SHINGLES
// ============================================

vector<string> getShingles(
    const vector<string>& words,
    int k
) {

    vector<string> shingles;

    if (
        (int)words.size() < k
    ) {

        return shingles;
    }

    for (
        int i = 0;
        i <= (int)words.size() - k;
        i++
    ) {

        string shingle;

        for (
            int j = 0;
            j < k;
            j++
        ) {

            if (j > 0) {

                shingle += " ";
            }

            shingle +=
                words[i + j];
        }

        shingles.push_back(
            shingle
        );
    }

    return shingles;
}


// ============================================
// HASH SHINGLES
// ============================================

vector<size_t> getShingleHashes(
    const vector<string>& shingles
) {

    vector<size_t> hashes;

    hash<string> hasher;

    for (
        size_t i = 0;
        i < shingles.size();
        i++
    ) {

        hashes.push_back(
            hasher(
                shingles[i]
            )
        );
    }

    return hashes;
}


// ============================================
// NAIVE WORD SIMILARITY
// ============================================

double calculateNaiveSimilarity(
    const vector<string>& words1,
    const vector<string>& words2
) {

    unordered_set<string> unique1;

    unordered_set<string> unique2;


    for (
        size_t i = 0;
        i < words1.size();
        i++
    ) {

        unique1.insert(
            words1[i]
        );
    }


    for (
        size_t i = 0;
        i < words2.size();
        i++
    ) {

        unique2.insert(
            words2[i]
        );
    }


    vector<string> list2;

    for (
        unordered_set<string>::iterator it =
            unique2.begin();

        it != unique2.end();

        ++it
    ) {

        list2.push_back(
            *it
        );
    }


    int commonWords = 0;


    for (
        unordered_set<string>::iterator it =
            unique1.begin();

        it != unique1.end();

        ++it
    ) {

        for (
            size_t j = 0;
            j < list2.size();
            j++
        ) {

            if (
                *it ==
                list2[j]
            ) {

                commonWords++;

                break;
            }
        }
    }


    int maximumUniqueWords =
        max(
            (int)unique1.size(),
            (int)unique2.size()
        );


    if (
        maximumUniqueWords == 0
    ) {

        return 0.0;
    }


    return
        (double)commonWords /
        maximumUniqueWords *
        100.0;
}


// ============================================
// EFFICIENT WORD SIMILARITY
// ============================================

double calculateEfficientSimilarity(
    const vector<string>& words1,
    const vector<string>& words2
) {

    unordered_set<string> set1;

    unordered_set<string> set2;


    for (
        size_t i = 0;
        i < words1.size();
        i++
    ) {

        set1.insert(
            words1[i]
        );
    }


    for (
        size_t i = 0;
        i < words2.size();
        i++
    ) {

        set2.insert(
            words2[i]
        );
    }


    int commonWords = 0;


    if (
        set1.size() >
        set2.size()
    ) {

        swap(
            set1,
            set2
        );
    }


    for (
        unordered_set<string>::iterator it =
            set1.begin();

        it != set1.end();

        ++it
    ) {

        if (
            set2.find(
                *it
            ) != set2.end()
        ) {

            commonWords++;
        }
    }


    int maximumUniqueWords =
        max(
            (int)set1.size(),
            (int)set2.size()
        );


    if (
        maximumUniqueWords == 0
    ) {

        return 0.0;
    }


    return
        (double)commonWords /
        maximumUniqueWords *
        100.0;
}


// ============================================
// SHARED WORD COUNT
// ============================================

int countSharedWords(
    const vector<string>& words1,
    const vector<string>& words2
) {

    unordered_set<string> set1;

    unordered_set<string> set2;


    for (
        size_t i = 0;
        i < words1.size();
        i++
    ) {

        set1.insert(
            words1[i]
        );
    }


    for (
        size_t i = 0;
        i < words2.size();
        i++
    ) {

        set2.insert(
            words2[i]
        );
    }


    if (
        set1.size() >
        set2.size()
    ) {

        swap(
            set1,
            set2
        );
    }


    int shared = 0;


    for (
        unordered_set<string>::iterator it =
            set1.begin();

        it != set1.end();

        ++it
    ) {

        if (
            set2.find(
                *it
            ) != set2.end()
        ) {

            shared++;
        }
    }


    return shared;
}


// ============================================
// SHARED SHINGLE COUNT
// ============================================

int countSharedShingles(
    const vector<size_t>& hashes1,
    const vector<size_t>& hashes2
) {

    unordered_set<size_t> set1(
        hashes1.begin(),
        hashes1.end()
    );

    unordered_set<size_t> set2(
        hashes2.begin(),
        hashes2.end()
    );


    if (
        set1.size() >
        set2.size()
    ) {

        swap(
            set1,
            set2
        );
    }


    int shared = 0;


    for (
        unordered_set<size_t>::iterator it =
            set1.begin();

        it != set1.end();

        ++it
    ) {

        if (
            set2.find(
                *it
            ) != set2.end()
        ) {

            shared++;
        }
    }


    return shared;
}


// ============================================
// SHINGLE SIMILARITY
// ============================================

double calculateShingleSimilarity(
    const vector<size_t>& hashes1,
    const vector<size_t>& hashes2
) {

    unordered_set<size_t> set1(
        hashes1.begin(),
        hashes1.end()
    );

    unordered_set<size_t> set2(
        hashes2.begin(),
        hashes2.end()
    );


    int intersection = 0;


    for (
        unordered_set<size_t>::iterator it =
            set1.begin();

        it != set1.end();

        ++it
    ) {

        if (
            set2.find(
                *it
            ) != set2.end()
        ) {

            intersection++;
        }
    }


    int unionSize =
        set1.size()
        +
        set2.size()
        -
        intersection;


    if (
        unionSize == 0
    ) {

        return 0.0;
    }


    return
        (double)intersection /
        unionSize *
        100.0;
}


// ============================================
// FINAL SCORE
// ============================================

double calculateFinalScore(
    double wordSimilarity,
    double shingleSimilarity
) {

    return
        wordSimilarity *
        WORD_WEIGHT
        +
        shingleSimilarity *
        SHINGLE_WEIGHT;
}


// ============================================
// STATUS
// ============================================

string getStatus(
    double score
) {

    if (
        score >=
        PLAGIARISM_THRESHOLD
    ) {

        return
            "PLAGIARISM SUSPECTED";
    }

    return
        "LOW SIMILARITY";
}


// ============================================
// BUILD SHINGLE INDEX
// ============================================

unordered_map<size_t, vector<int> >
buildShingleIndex(
    const vector<vector<size_t> >& allHashes
) {

    unordered_map<size_t, vector<int> >
        index;


    for (
        int doc = 0;
        doc < (int)allHashes.size();
        doc++
    ) {

        unordered_set<size_t> uniqueHashes(
            allHashes[doc].begin(),
            allHashes[doc].end()
        );


        for (
            unordered_set<size_t>::iterator it =
                uniqueHashes.begin();

            it != uniqueHashes.end();

            ++it
        ) {

            index[*it].push_back(
                doc
            );
        }
    }


    return index;
}


// ============================================
// SHINGLE CANDIDATES
// ============================================

set<pair<int, int> >
findShingleCandidates(
    const unordered_map<size_t, vector<int> >& index
) {

    set<pair<int, int> >
        candidates;


    for (
        unordered_map<size_t, vector<int> >::const_iterator
            entry = index.begin();

        entry != index.end();

        ++entry
    ) {

        const vector<int>& docs =
            entry->second;


        for (
            int i = 0;
            i < (int)docs.size();
            i++
        ) {

            for (
                int j = i + 1;
                j < (int)docs.size();
                j++
            ) {

                candidates.insert(
                    make_pair(
                        docs[i],
                        docs[j]
                    )
                );
            }
        }
    }


    return candidates;
}


// ============================================
// BUILD WORD INDEX
// ============================================

unordered_map<string, vector<int> >
buildWordIndex(
    const vector<vector<string> >& documents
) {

    unordered_map<string, vector<int> >
        index;


    for (
        int doc = 0;
        doc < (int)documents.size();
        doc++
    ) {

        unordered_set<string>
            uniqueWords;


        for (
            size_t i = 0;
            i < documents[doc].size();
            i++
        ) {

            if (
                (int)documents[doc][i].length()
                >=
                MIN_IMPORTANT_WORD_LENGTH
            ) {

                uniqueWords.insert(
                    documents[doc][i]
                );
            }
        }


        for (
            unordered_set<string>::iterator it =
                uniqueWords.begin();

            it != uniqueWords.end();

            ++it
        ) {

            index[*it].push_back(
                doc
            );
        }
    }


    return index;
}


// ============================================
// WORD CANDIDATES
// ============================================

set<pair<int, int> >
findWordCandidates(
    const unordered_map<string, vector<int> >& index
) {

    map<pair<int, int>, int>
        pairCounts;


    for (
        unordered_map<string, vector<int> >::const_iterator
            entry = index.begin();

        entry != index.end();

        ++entry
    ) {

        const vector<int>& docs =
            entry->second;


        for (
            int i = 0;
            i < (int)docs.size();
            i++
        ) {

            for (
                int j = i + 1;
                j < (int)docs.size();
                j++
            ) {

                pair<int, int> key =
                    make_pair(
                        docs[i],
                        docs[j]
                    );


                pairCounts[key]++;
            }
        }
    }


    set<pair<int, int> >
        candidates;


    for (
        map<pair<int, int>, int>::iterator it =
            pairCounts.begin();

        it != pairCounts.end();

        ++it
    ) {

        if (
            it->second >=
            MIN_SHARED_WORDS
        ) {

            candidates.insert(
                it->first
            );
        }
    }


    return candidates;
}


// ============================================
// COMBINE CANDIDATES
// ============================================

set<pair<int, int> >
combineCandidates(
    const set<pair<int, int> >& shingleCandidates,
    const set<pair<int, int> >& wordCandidates
) {

    set<pair<int, int> >
        result =
            shingleCandidates;


    for (
        set<pair<int, int> >::const_iterator it =
            wordCandidates.begin();

        it != wordCandidates.end();

        ++it
    ) {

        result.insert(
            *it
        );
    }


    return result;
}


// ============================================
// RUN NAIVE BENCHMARK
// ============================================

long long runNaiveBenchmark(
    const vector<vector<string> >& documents
) {

    volatile double preventOptimization = 0.0;


    auto start =
        chrono::high_resolution_clock::now();


    for (
        int i = 0;
        i < (int)documents.size();
        i++
    ) {

        for (
            int j = i + 1;
            j < (int)documents.size();
            j++
        ) {

            preventOptimization +=
                calculateNaiveSimilarity(
                    documents[i],
                    documents[j]
                );
        }
    }


    auto finish =
        chrono::high_resolution_clock::now();


    if (
        preventOptimization < -1
    ) {

        cout << "";
    }


    return
        chrono::duration_cast<
            chrono::microseconds
        >(
            finish - start
        ).count();
}


// ============================================
// RUN EFFICIENT BENCHMARK
// ============================================

long long runEfficientBenchmark(
    const vector<vector<string> >& documents,
    const vector<vector<size_t> >& hashes
) {

    volatile double preventOptimization = 0.0;


    auto start =
        chrono::high_resolution_clock::now();


    // ----------------------------------------
    // INDEX BUILDING
    // ----------------------------------------

    unordered_map<size_t, vector<int> >
        shingleIndex =
            buildShingleIndex(
                hashes
            );


    unordered_map<string, vector<int> >
        wordIndex =
            buildWordIndex(
                documents
            );


    // ----------------------------------------
    // CANDIDATE GENERATION
    // ----------------------------------------

    set<pair<int, int> >
        shingleCandidates =
            findShingleCandidates(
                shingleIndex
            );


    set<pair<int, int> >
        wordCandidates =
            findWordCandidates(
                wordIndex
            );


    set<pair<int, int> >
        candidates =
            combineCandidates(
                shingleCandidates,
                wordCandidates
            );


    // ----------------------------------------
    // ANALYZE ONLY CANDIDATES
    // ----------------------------------------

    for (
        set<pair<int, int> >::iterator it =
            candidates.begin();

        it != candidates.end();

        ++it
    ) {

        int i =
            it->first;


        int j =
            it->second;


        double wordSimilarity =
            calculateEfficientSimilarity(
                documents[i],
                documents[j]
            );


        double shingleSimilarity =
            calculateShingleSimilarity(
                hashes[i],
                hashes[j]
            );


        preventOptimization +=
            calculateFinalScore(
                wordSimilarity,
                shingleSimilarity
            );
    }


    auto finish =
        chrono::high_resolution_clock::now();


    if (
        preventOptimization < -1
    ) {

        cout << "";
    }


    return
        chrono::duration_cast<
            chrono::microseconds
        >(
            finish - start
        ).count();
}


// ============================================
// LARGE SCALE DATA GENERATOR
// ============================================

vector<vector<string> >
generateBenchmarkDocuments(
    int numberOfDocuments
) {

    vector<vector<string> >
        documents;


    vector<string> baseWords;


    baseWords.push_back("algorithm");

    baseWords.push_back("data");

    baseWords.push_back("computer");

    baseWords.push_back("system");

    baseWords.push_back("analysis");

    baseWords.push_back("document");

    baseWords.push_back("similarity");

    baseWords.push_back("information");

    baseWords.push_back("technology");

    baseWords.push_back("software");


    for (
        int i = 0;
        i < numberOfDocuments;
        i++
    ) {

        vector<string> words;


        int sharedGroup =
            i / 10;


        for (
            int j = 0;
            j < 20;
            j++
        ) {

            string word =
                baseWords[
                    j %
                    baseWords.size()
                ];


            word +=
                "_group_";


            word +=
                to_string(
                    sharedGroup
                );


            words.push_back(
                word
            );
        }


        for (
            int j = 0;
            j < 5;
            j++
        ) {

            words.push_back(
                "unique_" +
                to_string(i) +
                "_" +
                to_string(j)
            );
        }


        documents.push_back(
            words
        );
    }


    return documents;
}


// ============================================
// PREPARE BENCHMARK HASHES
// ============================================

vector<vector<size_t> >
prepareBenchmarkHashes(
    const vector<vector<string> >& documents
) {

    vector<vector<size_t> >
        hashes;


    for (
        size_t i = 0;
        i < documents.size();
        i++
    ) {

        vector<string> shingles =
            getShingles(
                documents[i],
                SHINGLE_SIZE
            );


        hashes.push_back(
            getShingleHashes(
                shingles
            )
        );
    }


    return hashes;
}


// ============================================
// FAIR PERFORMANCE BENCHMARK
// ============================================

void runFairPerformanceBenchmark() {

    cout << "\n";

    cout << "========================================\n";

    cout << " FAIR PERFORMANCE BENCHMARK\n";

    cout << "========================================\n\n";


    cout << left
         << setw(12)
         << "Documents"

         << setw(18)
         << "Possible Pairs"

         << setw(15)
         << "Candidates"

         << setw(15)
         << "Avoided"

         << setw(15)
         << "Reduction"

         << setw(18)
         << "Naive Time"

         << setw(18)
         << "Efficient Time"

         << "Speedup"
         << "\n";


    cout << string(
        130,
        '-'
    )
    << "\n";


    vector<int> testSizes;


    testSizes.push_back(20);

    testSizes.push_back(50);

    testSizes.push_back(100);

    testSizes.push_back(200);


    for (
        size_t t = 0;
        t < testSizes.size();
        t++
    ) {

        int n =
            testSizes[t];


        vector<vector<string> >
            documents =
                generateBenchmarkDocuments(
                    n
                );


        vector<vector<size_t> >
            hashes =
                prepareBenchmarkHashes(
                    documents
                );


        unordered_map<size_t, vector<int> >
            shingleIndex =
                buildShingleIndex(
                    hashes
                );


        unordered_map<string, vector<int> >
            wordIndex =
                buildWordIndex(
                    documents
                );


        set<pair<int, int> >
            shingleCandidates =
                findShingleCandidates(
                    shingleIndex
                );


        set<pair<int, int> >
            wordCandidates =
                findWordCandidates(
                    wordIndex
                );


        set<pair<int, int> >
            candidates =
                combineCandidates(
                    shingleCandidates,
                    wordCandidates
                );


        long long possiblePairs =
            (long long)n *
            (n - 1) /
            2;


        long long avoided =
            possiblePairs -
            candidates.size();


        double reduction =
            0.0;


        if (
            possiblePairs > 0
        ) {

            reduction =
                (double)avoided /
                possiblePairs *
                100.0;
        }


        long long naiveTime =
            runNaiveBenchmark(
                documents
            );


        long long efficientTime =
            runEfficientBenchmark(
                documents,
                hashes
            );


        double speedup =
            0.0;


        if (
            efficientTime > 0
        ) {

            speedup =
                (double)naiveTime /
                efficientTime;
        }


        cout
            << left
            << setw(12)
            << n

            << setw(18)
            << possiblePairs

            << setw(15)
            << candidates.size()

            << setw(15)
            << avoided

            << setw(14)
            << fixed
            << setprecision(2)
            << reduction
            << "%"

            << setw(18)
            << naiveTime

            << setw(18)
            << efficientTime;


        if (
            efficientTime > 0
        ) {

            cout
                << fixed
                << setprecision(2)
                << speedup
                << "x";

        } else {

            cout
                << "N/A";
        }


        cout << "\n";
    }


    cout << "\n";

    cout << "Time unit: microseconds\n";

    cout << "Reduction = percentage of possible pairs avoided.\n";

    cout << "Speedup = Naive Time / Efficient Time.\n";

    cout << "Efficient timing includes index construction, "
         << "candidate filtering and candidate analysis.\n";
}


// ============================================
// PRINT DETAILED REPORT
// ============================================

void printDetailedReport(
    const SimilarityResult& result,
    const vector<string>& filenames
) {

    cout << "\n";

    cout << "========================================\n";

    cout << " DETAILED PLAGIARISM REPORT\n";

    cout << "========================================\n\n";


    cout << "Document A: "
         << filenames[result.doc1]
         << "\n";


    cout << "Document B: "
         << filenames[result.doc2]
         << "\n\n";


    cout << "Word Similarity:       "
         << fixed
         << setprecision(2)
         << result.wordSimilarity
         << "%\n";


    cout << "Shingle Similarity:    "
         << result.shingleSimilarity
         << "%\n";


    cout << "Final Score:           "
         << result.finalScore
         << "%\n\n";


    cout << "Shared Words:          "
         << result.sharedWords
         << "\n";


    cout << "Shared Shingles:       "
         << result.sharedShingles
         << "\n\n";


    cout << "Threshold:             "
         << PLAGIARISM_THRESHOLD
         << "%\n\n";


    cout << "Decision: "
         << getStatus(
                result.finalScore
            )
         << "\n\n";


    if (
        result.finalScore >=
        PLAGIARISM_THRESHOLD
    ) {

        cout << "Reason:\n";

        cout << "The final similarity score is above "
             << "the configured plagiarism threshold.\n\n";

        cout << "The documents contain significant "
             << "overlap in their content.\n";

    } else {

        cout << "Reason:\n";

        cout << "The final similarity score is below "
             << "the configured plagiarism threshold.\n\n";

        cout << "The available evidence does not "
             << "indicate strong plagiarism.\n";
    }


    cout << "\n========================================\n";
}


// ============================================
// MAIN
// ============================================

vector<string> findTextDocuments() {

    vector<string> filenames;

    struct _finddata_t fileData;

    intptr_t searchHandle =
        _findfirst(
            "documents/*.txt",
            &fileData
        );

    if (
        searchHandle == -1
    ) {

        return filenames;
    }

    do {

        filenames.push_back(
            string("documents/") + fileData.name
        );

    } while (
        _findnext(
            searchHandle,
            &fileData
        ) == 0
    );

    _findclose(
        searchHandle
    );

    sort(
        filenames.begin(),
        filenames.end()
    );

    return filenames;
}

int main() {

    vector<string> filenames =
        findTextDocuments();


    vector<vector<string> >
        allDocuments;


    vector<vector<size_t> >
        allHashes;


    cout << "========================================\n";

    cout << " DOCUMENT SIMILARITY DETECTION SYSTEM\n";

    cout << "========================================\n\n";


    cout << "Documents selected: "
         << filenames.size()
         << "\n\n";


    // ============================================
    // LOAD DOCUMENTS
    // ============================================

    for (
        size_t i = 0;
        i < filenames.size();
        i++
    ) {

        string content =
            readDocument(
                filenames[i]
            );


        content =
            preprocessText(
                content
            );


        vector<string> words =
            getWords(
                content
            );


        vector<string> shingles =
            getShingles(
                words,
                SHINGLE_SIZE
            );


        vector<size_t> hashes =
            getShingleHashes(
                shingles
            );


        allDocuments.push_back(
            words
        );


        allHashes.push_back(
            hashes
        );


        cout << filenames[i]
             << " loaded successfully\n";


        cout << "Words: "
             << words.size()
             << "\n";


        cout << "Shingles: "
             << shingles.size()
             << "\n\n";
    }


    // ============================================
    // ALL POSSIBLE PAIRS
    // ============================================

    vector<pair<int, int> >
        allPairs;


    for (
        int i = 0;
        i < (int)filenames.size();
        i++
    ) {

        for (
            int j = i + 1;
            j < (int)filenames.size();
            j++
        ) {

            allPairs.push_back(
                make_pair(
                    i,
                    j
                )
            );
        }
    }


    // ============================================
    // INDEX CREATION
    // ============================================

    unordered_map<size_t, vector<int> >
        shingleIndex =
            buildShingleIndex(
                allHashes
            );


    unordered_map<string, vector<int> >
        wordIndex =
            buildWordIndex(
                allDocuments
            );


    // ============================================
    // CANDIDATES
    // ============================================

    set<pair<int, int> >
        shingleCandidates =
            findShingleCandidates(
                shingleIndex
            );


    set<pair<int, int> >
        wordCandidates =
            findWordCandidates(
                wordIndex
            );


    set<pair<int, int> >
        candidatePairs =
            combineCandidates(
                shingleCandidates,
                wordCandidates
            );


    // ============================================
    // NAIVE
    // ============================================

    cout << "===== NAIVE APPROACH =====\n\n";


    for (
        size_t i = 0;
        i < allPairs.size();
        i++
    ) {

        int doc1 =
            allPairs[i].first;


        int doc2 =
            allPairs[i].second;


        double similarity =
            calculateNaiveSimilarity(
                allDocuments[doc1],
                allDocuments[doc2]
            );


        cout << filenames[doc1]
             << " vs "
             << filenames[doc2]
             << "\n";


        cout << "Word Similarity: "
             << fixed
             << setprecision(2)
             << similarity
             << "%\n\n";
    }


    // ============================================
    // CANDIDATE FILTERING
    // ============================================

    cout << "===== CANDIDATE FILTERING =====\n\n";


    cout << "Shingle candidates: "
         << shingleCandidates.size()
         << "\n";


    cout << "Word candidates: "
         << wordCandidates.size()
         << "\n";


    cout << "Combined candidates: "
         << candidatePairs.size()
         << "\n";


    cout << "Total possible pairs: "
         << allPairs.size()
         << "\n";


    cout << "Pairs avoided: "
         << allPairs.size()
            -
            candidatePairs.size()
         << "\n\n";


    for (
        set<pair<int, int> >::iterator it =
            candidatePairs.begin();

        it != candidatePairs.end();

        ++it
    ) {

        cout << filenames[it->first]
             << " <--> "
             << filenames[it->second]
             << "\n";
    }


    cout << "\n";


    // ============================================
    // EFFICIENT ANALYSIS
    // ============================================

    cout << "===== EFFICIENT PLAGIARISM ANALYSIS =====\n\n";


    vector<SimilarityResult>
        results;


    for (
        set<pair<int, int> >::iterator it =
            candidatePairs.begin();

        it != candidatePairs.end();

        ++it
    ) {

        int doc1 =
            it->first;


        int doc2 =
            it->second;


        double wordSimilarity =
            calculateEfficientSimilarity(
                allDocuments[doc1],
                allDocuments[doc2]
            );


        double shingleSimilarity =
            calculateShingleSimilarity(
                allHashes[doc1],
                allHashes[doc2]
            );


        double finalScore =
            calculateFinalScore(
                wordSimilarity,
                shingleSimilarity
            );


        int sharedWords =
            countSharedWords(
                allDocuments[doc1],
                allDocuments[doc2]
            );


        int sharedShingles =
            countSharedShingles(
                allHashes[doc1],
                allHashes[doc2]
            );


        SimilarityResult result;


        result.doc1 =
            doc1;


        result.doc2 =
            doc2;


        result.wordSimilarity =
            wordSimilarity;


        result.shingleSimilarity =
            shingleSimilarity;


        result.finalScore =
            finalScore;


        result.sharedWords =
            sharedWords;


        result.sharedShingles =
            sharedShingles;


        results.push_back(
            result
        );


        cout << filenames[doc1]
             << " vs "
             << filenames[doc2]
             << "\n";


        cout << "   Word Similarity: "
             << fixed
             << setprecision(2)
             << wordSimilarity
             << "%\n";


        cout << "   Shingle Similarity: "
             << shingleSimilarity
             << "%\n";


        cout << "   Final Score: "
             << finalScore
             << "%\n";


        cout << "   Status: "
             << getStatus(
                    finalScore
                )
             << "\n\n";
    }


    // ============================================
    // SORT
    // ============================================

    sort(
        results.begin(),
        results.end(),
        [](const SimilarityResult& a,
           const SimilarityResult& b) {

            return
                a.finalScore >
                b.finalScore;
        }
    );


    // ============================================
    // TOP K
    // ============================================

    cout << "===== TOP "
         << TOP_K
         << " MOST SIMILAR PAIRS =====\n\n";


    int limit =
        min(
            TOP_K,
            (int)results.size()
        );


    for (
        int i = 0;
        i < limit;
        i++
    ) {

        cout << i + 1
             << ". "
             << filenames[
                    results[i].doc1
                ]
             << " vs "
             << filenames[
                    results[i].doc2
                ]
             << "\n";


        cout << "   Final Score: "
             << fixed
             << setprecision(2)
             << results[i].finalScore
             << "%\n";


        cout << "   Status: "
             << getStatus(
                    results[i].finalScore
                )
             << "\n\n";
    }


    // ============================================
    // FAIR PERFORMANCE TEST
    // ============================================

    runFairPerformanceBenchmark();


    // ============================================
    // DETAILED REPORT
    // ============================================

    if (
        !results.empty()
    ) {

        printDetailedReport(
            results[0],
            filenames
        );
    }


    // ============================================
    // FINAL SUMMARY
    // ============================================

    cout << "\n";

    cout << "===== FINAL SUMMARY =====\n\n";


    cout << "Documents processed: "
         << filenames.size()
         << "\n";


    cout << "Total possible pairs: "
         << allPairs.size()
         << "\n";


    cout << "Candidate pairs: "
         << candidatePairs.size()
         << "\n";


    cout << "Pairs avoided: "
         << allPairs.size()
            -
            candidatePairs.size()
         << "\n";


    cout << "Plagiarism threshold: "
         << fixed
         << setprecision(2)
         << PLAGIARISM_THRESHOLD
         << "%\n";


    cout << "\n========================================\n";

    cout << " PROJECT RUN COMPLETED SUCCESSFULLY\n";

    cout << "========================================\n";


    return 0;
}