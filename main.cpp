#include "util.hpp"
using namespace std;

/* void dfsLol(Node *root)
{
    if(!root) return;

    firstPos(root);
    lastPos(root);
    nullable(root);

    dfsLol(root->left);
    dfsLol(root->right);
} */

int main()
{
    fstream fOut("output.txt", ios::out | ios::trunc);


    // TOKENISER ...
    cout << "--> Reading Lex Definations ..." << endl;
    fstream fLex;
    fLex.open("input_lex.txt", ios::in);
    vector<pair<string, string>> reStringTokenPairList = inputReString(fLex);
    fLex.close();
    fOut << "RAW READ LEX DEFINATIONS (STRING) -->" << endl;
    outputReString(reStringTokenPairList, fOut);
    fOut << endl << "___________________________________________________________________________" << endl << endl;
    cout << "--> Reading Lex Definations Done ..." << endl << endl;
    
    cout << "--> Processing Concat ..." << endl;
    vector<pair<vector<Character>, string>> reTokenPairList = addConcatList(reStringTokenPairList);
    fOut << "CONCAT PROCESSED LEX DEFINATIONS -->" << endl;
    outputReList(reTokenPairList, fOut);
    fOut << endl << "___________________________________________________________________________" << endl << endl;
    cout << "--> Processing Concat Done ..." << endl << endl;

    cout << "--> Processing Postfix ..." << endl;
    for(pair<vector<Character>, string> &p: reTokenPairList) p.first = convertPostfix(p.first); 
    fOut << "POSTFIX PROCESSED LEX DEFINATIONS -->" << endl;
    outputReList(reTokenPairList, fOut);
    fOut << endl << "___________________________________________________________________________" << endl << endl;
    cout << "--> Processing Postfix Done ..." << endl << endl;

    cout << "--> Generating Combined Syntax Tree ..." << endl;
    pair<Node *, unordered_map<Node *, string>> lols1 = generateCombinedSyntaxTree(reTokenPairList);
    Node *masterTree = lols1.first;
    unordered_map<Node *, string> finalNodeToTokenTypeMap = lols1.second;
    fOut << "COMBINED SYNTAX TREE -->" << endl;
    fOut << "Pre-Order Traversal -> "; printPreorderTraversal(masterTree, fOut); fOut << endl;
    fOut << "Final Node to Token Type Map -->" << endl;
    for(auto &p: finalNodeToTokenTypeMap) fOut << p.first->data << "  -->  " << p.second << endl;
    fOut << endl << "___________________________________________________________________________" << endl << endl;
    cout << "--> Generating Combined Syntax Tree Done ..." << endl << endl;

    cout << "--> Enumerating Leaf Nodes and computing nullable, first, last, follow ..." << endl;
    enumerateLeafNodes(masterTree);
    // dfsLol(masterTree);
    // cout << "LOL" << endl;
    dfsComputeFollowPos(masterTree);
    fOut << "MASTER SYNTAX TREE SPECIFICATIONS -->" << endl;
    for(int i = 0; i < curIndex; i++)
    {
        fOut << "#" << i << " -->" << endl;
        Node *n = nodeOfIndex[i];
        fOut << "Nullable: " << (nullable(n)? "True": "False") << endl;
        fOut << "First Pos: "; for(Node *f: firstPos(n)) fOut << indexOfNode[f] << ", "; fOut << endl;
        fOut << "Last Pos: "; for(Node *l: lastPos(n)) fOut << indexOfNode[l] << ", "; fOut << endl;
        fOut << "Follow Pos: "; for(Node *fl: followPos(n)) fOut << indexOfNode[fl] << ", "; fOut << endl;
        fOut << endl << endl;
    }
    fOut << endl << "___________________________________________________________________________" << endl << endl;
    cout << "--> Enumerating Leaf Nodes and computing nullable, first, last, follow Done ..." << endl << endl;

    cout << "--> Generating DFA ..." << endl;
    unordered_set<char> symbolsRe = getSymbolsFromRe(reTokenPairList);
    pair<pair<vector<vector<pair<char, int>>>, unordered_map<int, string>>, vector<unordered_set<Node *>>> lols2 = generateDFAFromSyntaxTree(masterTree, symbolsRe, finalNodeToTokenTypeMap);
    vector<vector<pair<char, int>>> dfa = lols2.first.first;
    vector<unordered_map<char, int>> dfaMap = makeDfaMap(dfa);
    unordered_map<int, string> finalStatesDfaMap = lols2.first.second;
    vector<unordered_set<Node *>> dfaAlias = lols2.second;
    fOut << "DFA DEFINATIONS -->" << endl;
    fOut << "Symbols from RE: "; for(char ch: symbolsRe) fOut << ch << " , "; fOut << endl;
    fOut << "DFA -->" << endl;
    displayDFA(dfa, finalStatesDfaMap, fOut);
    fOut << endl << "Alias -->" << endl;
    for(int i = 0; i < dfaAlias.size(); i++)
    {
        fOut << "#" << i << " -> ";
        for(Node *n: dfaAlias[i]) fOut << indexOfNode[n] << " ,";
        fOut << endl;
    }
    fOut << endl << "___________________________________________________________________________" << endl << endl;
    cout << "--> Generating DFA Done ..." << endl << endl;

    cout << "--> Reading Input Character Stream ..." << endl;
    fstream fChar;
    fChar.open("input_text.txt", ios::in);
    string inputCharStream = readFullInputCharacterStream(fChar);
    fChar.close();
    fOut << "READ INPUT CHARACTER STREAM -->" << endl;
    fOut << inputCharStream << "[END]" << endl;
    fOut << endl << "___________________________________________________________________________" << endl << endl;
    cout << "--> Reading Input Character Stream Done ..." << endl << endl;

    cout << "--> Tokenizing using DFA ..." << endl;
    vector<Token> inputTokens = tokeniseCharacterStreamUsingDfa(inputCharStream, dfaMap, finalStatesDfaMap);
    fChar.close();
    fOut << "TOKENS --> Size = ";
    fstream fTokens;
    printInputTokens(inputTokens, fOut, true);
    fTokens.open("input_tokens.txt", ios::out | ios::trunc);
    printInputTokens(inputTokens, fTokens, true);
    fTokens.close();
    fOut << endl << "___________________________________________________________________________" << endl << endl;
    cout << "--> Tokenizing using DFA Done ..." << endl << endl;


    fOut << endl << endl << endl << "___________________________________________________________________________" << endl;
    fOut << "___________________________________________________________________________" << endl;
    fOut << "___________________________________________________________________________" << endl;
    fOut << "___________________________________________________________________________" << endl << endl << endl<< endl << endl;


    // LL(1) PARSING ...
    cout << "--> Reading Productions ..." << endl;
    fstream fProd("input_prod.txt", ios::in);
    pair<vector<string>, Item> lol1 = readProductionsAsString(fProd);
    vector<string> prodStr = lol1.first;
    Item startSymbol = lol1.second;
    fProd.close();
    cout << "--> Reading Done ..." << endl << endl;

    cout << "--> Testing String Read ..." << endl;
    fOut << "RAW READ PRODUCTIONS (STRING) -->  [ Size = " << prodStr.size() << " ]" << endl;
    for(string st: prodStr) fOut << st << endl; fOut << endl << "___________________________________________________________________________" << endl << endl;
    cout << "--> Testing Done ..." << endl << endl;

    cout << "--> Parsing Productions ..." << endl;
    pair<Map, pair<Set, Set>> lol2 = parseProductionsfromInput(prodStr);
    Map prodMap = lol2.first;
    Set symbolSet = lol2.second.first;
    Set terminalSet = lol2.second.second;
    fOut << "READ PRODUCTIONS [ Start Symbol = " << startSymbol << " ] -->" << endl;
    printProductionList(prodMap, symbolSet, terminalSet, fOut); fOut << endl << "___________________________________________________________________________" << endl << endl;
    cout << "--> Parsing Done ..." << endl << endl;

    cout << "--> Removing left recursions ..." << endl;
    pair<Map, Set> lol3 = removeImmediateLeftRecursion(prodMap);
    prodMap = lol3.first;
    Set extraSymbols = lol3.second;
    for(Item symbol :extraSymbols) symbolSet.insert(symbol);
    Action::setProdMap(&prodMap);
    fOut << "LEFT RECURSION REMOVED PRODUCTIONS -->" << endl;
    printProductionList(prodMap, symbolSet, terminalSet, fOut); fOut << endl << "___________________________________________________________________________" << endl << endl;
    cout << "--> Left Recursions Removed ..." << endl << endl;

    cout << "--> Generating nullable map ..." << endl;
    MapNullable mapNullable = generateNullableMap(prodMap, symbolSet);
    fOut << "NULLABLE MAP -->" << endl;
    for(auto lol: mapNullable){ Item i = lol.first; fOut << i << " -> " << ((lol.second)? "True": "False") << endl; }
    fOut << endl << "___________________________________________________________________________" << endl << endl;
    cout << "--> Generation done ..." << endl << endl;

    cout << "--> Generating first map ..." << endl;
    MapFirst mapFirst = generateFirstFromProductions(prodMap, symbolSet, mapNullable);
    fOut << "FIRST MAP -->" << endl;
    for(auto lol: mapFirst){ Item i = lol.first; fOut << i << " -> { "; bool b = false; for(Item a: lol.second){ if(b) fOut << ", "; b = true; fOut << a << " "; } fOut << "}" << endl; }
    fOut << endl << "___________________________________________________________________________" << endl << endl;
    cout << "--> Generation done ..." << endl << endl;

    cout << "--> Generating follow map ..." << endl;
    MapFollow mapFollow = generateFollowFromProductions(prodMap, symbolSet, mapNullable, mapFirst, startSymbol);
    fOut << "FOLLOW MAP -->" << endl;
    for(auto lol: mapFollow){ Item i = lol.first; fOut << i << " -> { "; bool b = false; for(Item a: lol.second){ if(b) fOut << ", "; b = true; fOut << a << " "; } fOut << "}" << endl; }
    fOut << endl << "___________________________________________________________________________" << endl << endl;
    cout << "--> Generation done ..." << endl << endl;

    cout << "--> Generating LL(1) Parsing Table ..." << endl;
    ParsingTable parsingTable = generateLl1ParsingTable(prodMap, symbolSet, mapNullable, mapFirst, mapFollow);
    fOut << "LL(1) PARSING TABLE -->" << endl << endl;
    printParsingTable(parsingTable, symbolSet, terminalSet, fOut);
    fOut << endl << "___________________________________________________________________________" << endl << endl;
    cout << "--> Generation done ..." << endl << endl;

    cout << "--> Augmenting LL(1) Parsing Table with error recovery ..." << endl;
    augmentLl1ParsingTableWithErrorRecovery(prodMap, symbolSet, terminalSet, mapNullable, mapFirst, mapFollow, parsingTable);
    fOut << "AUGMENTED LL(1) PARSING TABLE -->" << endl << endl;
    printParsingTable(parsingTable, symbolSet, terminalSet, fOut);
    fOut << endl << "___________________________________________________________________________" << endl << endl;
    cout << "--> Augmentation done ..." << endl << endl;

    cout << "--> Reading Token Stream ..." << endl;
    fstream fInTokenStream("input_tokens.txt");
    vector<Token> tokenStream = readTokenStreamFromFile(fInTokenStream);
    fInTokenStream.close();
    fOut << "READ TOKEN STREAM -->" << endl;
    printReadTokenStream(tokenStream, fOut);
    fOut << endl << "___________________________________________________________________________" << endl << endl;
    cout << "--> Reading done ..." << endl << endl;

    cout << "--> Parsing using LL(1) Parsing Table ..." << endl;
    pair<pair<bool, string>, ParsingHistory> lol4 = parseTokenStreamUsingParsingTable(tokenStream, symbolSet, terminalSet, startSymbol, parsingTable, prodMap);
    bool errorFoundParsingResult = lol4.first.first;
    string parsingErrorInfo = lol4.first.second;
    ParsingHistory parsingHistory = lol4.second;
    fOut << "PARSING RESULT: " << ((!errorFoundParsingResult)? "TRUE": "FALSE") << endl;
    fOut << "PARSING ERROR INFO: " << parsingErrorInfo << endl << endl;
    printParsingHistory(parsingHistory, fOut);
    fOut << endl << "___________________________________________________________________________" << endl << endl;
    cout << "--> Parsing done ..." << endl << endl;


    fOut.close();

    cout << "DONE !!!" << endl;

    return 0;
}