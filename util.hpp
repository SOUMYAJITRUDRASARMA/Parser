#include <bits/stdc++.h>
using namespace std;




// ........................................................................................................................................................
// ........................................................................................................................................................
// RE TO DFA ..............................................................................................................................................
// ........................................................................................................................................................
// ........................................................................................................................................................

struct Node
{
    char data;
    Node *left, *right;

    Node(char data = '\0', Node *left = nullptr, Node *right = nullptr){ this->data = data; this->left = left; this->right = right; }
};


unordered_map<Node *, bool> dpNullable;
unordered_map<Node *, unordered_set<Node *>> dpFirstPos, dpLastPos, dpFollowPos;
unordered_map<Node *, int> indexOfNode;  vector<Node *> nodeOfIndex;  int curIndex = 0;
const char EPSILON_DFA = '\0', DELIM = '#';
typedef pair<char, bool> Character;  // <value of char , isOperator>


/* string addConcat(string &re)
{
    string ans;
    bool escapeOn = false;
    for(int i = 0; i < re.size() - 1; i++)
    {
        ans.push_back(re[i]);

        // if(escapeOn) escapeOn = false;
        // else if(re[i] == '\\') escapeOn = true;
        
        // if(((re[i] != '(' && re[i] != '+' && re[i] != '.') || !escapeOn) && 
        //      re[i + 1] != '+' && re[i + 1] != '.' && re[i + 1] != '*' && re[i + 1] != ')') ans.push_back('.');

        if(escapeOn)
        {
            if(i != re.size() + 1 && re[i + 1] != '+' && re[i + 1] != '.' && re[i + 1] != '*' && re[i + 1] != ')') ans.push_back('.');
            escapeOn = false;
        }
        else
        {
            if(re[i] == '\\') escapeOn = true;
            else
            {
                if(re[i] != '(' && re[i] != '+' && re[i] != '.' && 
                   re[i + 1] != '+' && re[i + 1] != '.' && re[i + 1] != '*' && re[i + 1] != ')') ans.push_back('.');
            }
        }

        // if((re[i] != '(' && re[i] != '+' && re[i] != '.' && re[i] != '\\' && 
        //     re[i + 1] != '+' && re[i + 1] != '.' && re[i + 1] != '*' && re[i + 1] != ')') || 
        //         (i && re[i - 1] =='\\' && (i == 1 || re[i - 2] != '\\'))) ans.push_back('.');
    }
    ans.push_back(re.back());

    return ans;
} */

bool isOperatorAugmented(char ch)
{
    return ch == '+' || ch == '.' || ch == '*' || ch == '(' || ch == ')';
}

vector<pair<string, string>> inputReString(fstream &f)
{
    vector<pair<string, string>> ans;  // re to token type ...
    string tokenType;
    int n;
    f >> n;

    char buffer[300];
    while(n--)
    {
        f.getline(buffer, 300);  // flushing newline ...
        f.getline(buffer, 300);
        f >> tokenType;
        ans.push_back({buffer, tokenType});
    }

    return ans;
}

void outputReString(vector<pair<string, string>> &reStringTokenPairList, fstream &f)
{
    f << "#Registered Tokens: " << reStringTokenPairList.size() << endl << endl;
    for(pair<string, string> &p: reStringTokenPairList) f << p.first << "  -->  " << p.second << endl;
}

vector<Character> addConcat(string &re)
{
    vector<Character> ans;
    bool escapeOn = false;
    for(int i = 0; i < re.size(); i++)
    {
        // if(escapeOn || re[i] != '\\') ans.push_back({re[i], isOperatorAugmented(re[i])});

        if(escapeOn)
        {
            if(re[i] == 'n') ans.push_back({'\n', false});  // newline
            else if(re[i] == 't') ans.push_back({'\t', false});  // tab
            else ans.push_back({re[i], false});

            if(i != re.size() - 1 && re[i + 1] != '+' && re[i + 1] != '.' && re[i + 1] != '*' && re[i + 1] != ')') ans.push_back({'.', true});
            escapeOn = false;
        }
        else
        {
            if(re[i] == '\\') escapeOn = true;
            else
            {
                ans.push_back({re[i], isOperatorAugmented(re[i])});
                if(i != re.size() - 1 && re[i] != '(' && re[i] != '+' && re[i] != '.' && 
                   re[i + 1] != '+' && re[i + 1] != '.' && re[i + 1] != '*' && re[i + 1] != ')') ans.push_back({'.', true});
            }
        }
    }
    // ans.push_back({re.back(), isOperatorAugmented(re.back())});

    return ans;
}

vector<pair<vector<Character>, string>> addConcatList(vector<pair<string, string>> &reStringTokenPairList)
{
    vector<pair<vector<Character>, string>> reTokenPairList;
    for(pair<string, string> &p: reStringTokenPairList){ reTokenPairList.push_back({addConcat(p.first), p.second}); 
        // cout << "DONE -> " << p.first << " -> " << p.second << endl;
    }
    return reTokenPairList;
}

void printCharacter(Character ch, ostream &o)
{
    o << "{" << ((ch.first == '\n')? "\\n": (ch.first == '\t')? "\\t": string(1, ch.first)) << " , " << ((ch.second)? "O": "C") << "}";
}

void printCharacterVector(vector<Character> &v, ostream &o)
{
    bool lol = false;
    o << "[ "; for(Character c: v){ if(lol) o << " ,"; lol = true; printCharacter(c, o); } o << " ]";
}

void outputReList(vector<pair<vector<Character>, string>> &reTokenPairList, fstream &f)
{
    f << "#Registered Tokens: " << reTokenPairList.size() << endl << endl;
    for(pair<vector<Character>, string> &p: reTokenPairList){ printCharacterVector(p.first, f); f << "  -->  " << p.second << endl; }
}

int precedenceOperator(char op)
{
    if(op == '+') return 1;
    if(op == '.') return 2;
    if(op == '*') return 3;
    else return -1;
}

vector<Character> convertPostfix(vector<Character> &re)
{
    stack<char> st;
    vector<Character> ans;

    for(Character ch: re)
    {
        if(ch.first == '(' && ch.second) st.push(ch.first);
        else if(ch.first == ')' && ch.second){ while(st.top() != '('){ ans.push_back({st.top(), true}); st.pop(); } st.pop(); }
        else if(ch.second){ while(!st.empty() && precedenceOperator(ch.first) <= precedenceOperator(st.top())){ ans.push_back({st.top(), true}); st.pop(); } st.push(ch.first); }  // Operator
        else ans.push_back({ch.first, false});
    }

    while(!st.empty()){ ans.push_back({st.top(), true}); st.pop(); }

    return ans;
}

Node *generateSyntaxTree(vector<Character> &postRe)
{
    stack<Node *> st;

    for(Character ch: postRe)
    {
        // cout << ch << " | " << st.size() << endl;
        if(ch.second)
        {
            if(ch.first == '*')   // Unary Operator
            {
                if(st.size() < 1) return nullptr;  // Error
                Node *n = st.top(); st.pop();
                st.push(new Node(ch.first, n));
            }
            else   // Binary Operator
            {
                if(st.size() < 2) return nullptr;  // Error
                Node *nRight = st.top(); st.pop();
                Node *nLeft = st.top(); st.pop();
                st.push(new Node(ch.first, nLeft, nRight));
            }
        }
        else st.push(new Node(ch.first));
    }

    // cout << "Lol: " << st.size() << endl;

    if(st.size() != 1) return nullptr;  // Error
    else return new Node('.', st.top(), new Node(DELIM));
}


pair<Node *, unordered_map<Node *, string>> generateCombinedSyntaxTree(vector<pair<vector<Character>, string>> &reList)
{
    Node *masterTree = nullptr;
    unordered_map<Node *, string> finalNodeToTokenTypeMap;
    for(pair<vector<Character>, string> &reTokenPair: reList)
    {
        Node *syntaxTree = generateSyntaxTree(reTokenPair.first);
        string tokenType = reTokenPair.second;

        if(masterTree == nullptr) masterTree = syntaxTree;
        else masterTree = new Node('+', masterTree, syntaxTree);

        finalNodeToTokenTypeMap[syntaxTree->right] = tokenType;
    }

    return {masterTree, finalNodeToTokenTypeMap};
}


void printPreorderTraversal(Node *root, ostream &o)
{
    if(!root) return;

    o << " [ " << ((root->data == '\n')? "\\n": (root->data == '\t')? "\\t": string(1, root->data));
    printPreorderTraversal(root->left, o);
    printPreorderTraversal(root->right, o); o << " ]";
}


bool nullable(Node *n)
{
    if(!n->left && !n->right) return n->data == EPSILON_DFA;
    else if(dpNullable.find(n) != dpNullable.end()) return dpNullable[n];

    if(n->data == '+') return dpNullable[n] = nullable(n->left) || nullable(n->right);
    else if(n->data == '.') return dpNullable[n] = nullable(n->left) && nullable(n->right);
    else if(n->data == '*') return dpNullable[n] = true;
    else return false;
}


unordered_set<Node *> unionSet(unordered_set<Node *> s1, unordered_set<Node *> s2)
{
    unordered_set<Node *> ans;
    for(Node *n: s1) ans.insert(n);
    for(Node *n: s2) ans.insert(n);
    return ans;
}


unordered_set<Node *> firstPos(Node *n)
{
    if(!n->left && !n->right){ if(n->data == EPSILON_DFA) return unordered_set<Node *>(); 
                               else return unordered_set<Node *>({n}); }
    else if(dpFirstPos.find(n) != dpFirstPos.end()) return dpFirstPos[n];

    if(n->data == '+') return dpFirstPos[n] = unionSet(firstPos(n->left), firstPos(n->right));
    else if(n->data == '.'){ if(nullable(n->left)) return dpFirstPos[n] = unionSet(firstPos(n->left), firstPos(n->right));
                             else return dpFirstPos[n] = firstPos(n->left); }
    else if(n->data == '*') return dpFirstPos[n] = firstPos(n->left);
    else return unordered_set<Node *>();
}


unordered_set<Node *> lastPos(Node *n)
{
    if(!n->left && !n->right){ if(n->data == EPSILON_DFA) return unordered_set<Node *>(); 
                               else return unordered_set<Node *>({n}); }
    else if(dpLastPos.find(n) != dpLastPos.end()) return dpLastPos[n];

    if(n->data == '+') return dpLastPos[n] = unionSet(lastPos(n->left), lastPos(n->right));
    else if(n->data == '.'){ if(nullable(n->right)) return dpLastPos[n] = unionSet(lastPos(n->left), lastPos(n->right));
                             else return dpLastPos[n] = lastPos(n->right); }
    else if(n->data == '*') return dpLastPos[n] = lastPos(n->left);
    else return unordered_set<Node *>();
}


void dfsComputeFollowPos(Node *root)
{
    if(!root) return;

    // Since terminal nodes can have . and * , its better to check for non-leaf nodes ...
    if(root->data == '.' && root->left && root->right)
    {
        for(Node *l: lastPos(root->left))
            for(Node *f: firstPos(root->right))
                dpFollowPos[l].insert(f);
    }
    else if(root->data == '*' && root->left)
    {
        for(Node *l: lastPos(root->left))
            for(Node *f: firstPos(root->left))
                dpFollowPos[l].insert(f);
    }

    dfsComputeFollowPos(root->left);
    dfsComputeFollowPos(root->right);
}


unordered_set<Node *> followPos(Node *n)
{
    return dpFollowPos[n];
}


void enumerateLeafNodes(Node *n)
{
    // In PreOrder Fashion ...
    if(!n) return;

    if(!n->left && !n->right){ indexOfNode[n] = curIndex++; nodeOfIndex.push_back(n); return; }
    else{ enumerateLeafNodes(n->left); enumerateLeafNodes(n->right); }
}


unordered_set<char> getSymbolsFromRe(vector<pair<vector<Character>, string>> &reList)
{
    unordered_set<char> symbols;
    for(pair<vector<Character>, string> &re: reList) for(Character ch: re.first) if(!ch.second) symbols.insert(ch.first);
    return symbols;
}


struct MySetHasher
{
public:
    size_t operator () (const unordered_set<Node *> & s) const
    {
        size_t ans = 0;
        for(Node *n: s) ans ^= ((size_t)n);
        return ans % 2000;
    }
};


pair<pair<vector<vector<pair<char, int>>>, unordered_map<int, string>>, vector<unordered_set<Node *>>> generateDFAFromSyntaxTree(Node *root, unordered_set<char> symbols, unordered_map<Node *, string> &endNodeToTokenTypeMap, bool log = true)
{
    // returns (graph, final_states) first , then returns actual alias of enumeration to unordered_set of nodes ...
    // 0th state is always starting state ...
    vector<unordered_set<Node *>> dStates;  // used to keep track of marked dStates ...
    unordered_map<unordered_set<Node *>, int, MySetHasher> indexOfState;
    unordered_map<unordered_set<Node *>, vector<pair<char, unordered_set<Node *>>>, MySetHasher> dTrans;  // Weighted Graph (char, newState [as a set]) ...
    unordered_set<Node *> curState;

    int cur = 0;
    dStates.push_back({firstPos(root)});
    dTrans[{firstPos(root)}];
    indexOfState[{firstPos(root)}] = 0;

    while(cur < dStates.size())
    {
        curState = dStates[cur++];

        for(char symbol: symbols)
        {
            unordered_set<Node *> nextState;

            for(Node *n: curState)
                if(n->data == symbol)
                    for(Node *f: followPos(n))
                        nextState.insert(f);

            if(!nextState.empty())
            {
                // if nextState is empty , means dead state ...
                if(dTrans.find(nextState) == dTrans.end()) dTrans[nextState], dStates.push_back(nextState), indexOfState[nextState] = dStates.size() - 1;
                dTrans[curState].push_back({symbol, nextState});
            }
        }
    }

    vector<vector<pair<char, int>>> dfa(dStates.size());
    unordered_map<int, string> finalStates;
    vector<string> multipleDefinationLogs;
    for(int i = 0; i < dStates.size(); i++)
    {
        // if(dStates[i].find(endNode) != dStates[i].end()) finalStates.insert(i);
        for(Node *n: dStates[i]) if(endNodeToTokenTypeMap.find(n) != endNodeToTokenTypeMap.end())
        {
            if(finalStates.find(i) != finalStates.end()) multipleDefinationLogs.push_back("Node #" + to_string(i) + " -> " + endNodeToTokenTypeMap[n]);
            else finalStates[i] = endNodeToTokenTypeMap[n];
        }

        for(pair<char, unordered_set<Node *>> &edge: dTrans[dStates[i]])
            dfa[i].push_back({edge.first, indexOfState[edge.second]});
    }

    if(log && !multipleDefinationLogs.empty()){ cout << "MULTIPLE DEFINATIONS OF DFA TOKENS FOUND -->" << endl; for(string st: multipleDefinationLogs) cout << "  " << st << endl; cout << endl; }

    return {{dfa, finalStates}, dStates};
}


void displayDFA(vector<vector<pair<char, int>>> &dfa, unordered_map<int, string> &finalStates, ostream &o)
{
    for(int i = 0; i < dfa.size(); i++)
    {
        if(i == 0) o << "-> "; else if(finalStates.find(i) != finalStates.end()) o << "** (" << finalStates[i] << ") "; else o << "   ";
        o << "#" << i << " -->  ";
        for(pair<char, int> edge: dfa[i]) o << "(" << ((edge.first == '\n')? "\\n": (edge.first == '\t')? "\\t": string(1, edge.first)) << " , " << edge.second << ")  |  ";
        o << " &SubCnt: " << dfa[i].size() << endl;
    }

    o << endl << "Size:  " << dfa.size() << endl;
}


vector<unordered_map<char, int>> makeDfaMap(vector<vector<pair<char, int>>> &dfa)
{
    vector<unordered_map<char, int>> dfaMap(dfa.size());
    for(int i = 0; i < dfa.size(); i++) for(pair<char, int> p: dfa[i]) dfaMap[i][p.first] = p.second;
    return dfaMap;
}


string readFullInputCharacterStream(fstream &f)
{
    string ans;
    char ch;
    while(f.get(ch)) ans.push_back(ch);
    return ans;
}












// ........................................................................................................................................................
// ........................................................................................................................................................
// LL(1) PARSER ...........................................................................................................................................
// ........................................................................................................................................................
// ........................................................................................................................................................

#define EPSILON 'E'




class Item
{
public:
    string val;
    bool isSymbol;

    Item(){ this->val = ""; this->isSymbol = false; }
    Item(string val, bool isSymbol){ this->val = val; this->isSymbol = isSymbol; }
    bool operator == (const Item &i) const{ return this->val == i.val && this->isSymbol == i.isSymbol; }
    bool operator != (const Item &i) const{ return !(this->val == i.val && this->isSymbol == i.isSymbol); }
    bool isEpsilon(){ return !isSymbol && val.size() == 1 && val[0] == EPSILON; }
    bool isInvalid(){ return val.empty(); }
    string toString(){ if(isInvalid()) return "I(INVALID)"; string ans; if(isSymbol) ans = "S("; else ans =  "T("; ans += val + ")"; return ans; }

    friend ostream & operator << (ostream &os, Item &i);
};

ostream & operator << (ostream &os, Item &i)
{
    if(i.isInvalid()){ os << "I(INVALID)"; return os; }

    if(i.isSymbol) os << "S(";
    else os << "T(";
    os << i.val << ")";
    return os;
}

struct ItemHasher
{
public:
    size_t operator () (const Item &i) const
    {
        return hash<string>()(i.val) ^ hash<bool>()(i.isSymbol);
    }
};

typedef unordered_map<Item, vector<vector<Item>>, ItemHasher> Map; 
typedef unordered_set<Item, ItemHasher> Set; 
typedef unordered_map<Item, Set, ItemHasher> MapFirst; 
typedef unordered_map<Item, Set, ItemHasher> MapFollow; 
typedef unordered_map<Item, bool, ItemHasher> MapNullable; 




class Action
{
public:
    const static int PRODUCE = 1, POP = 2, SCAN = 3, MATCH_TOKEN = 4;

    static Map *prodMap;
    int type;
    Item prodLhs;
    int prodRhsIndex;

    Action(){ type = 0; }
    Action(int type){ this->type = type; }
    Action(int type, Item prodLhs, int prodRhsIndex){ this->type = type; this->prodLhs = prodLhs; this->prodRhsIndex = prodRhsIndex; }
    Action(Item prodLhs, int prodRhsIndex){ this->type = PRODUCE; this->prodLhs = prodLhs; this->prodRhsIndex = prodRhsIndex; }
    static void setProdMap(Map *prodMapActual){ Action::prodMap = prodMapActual; }
    bool operator == (const Action &a) const{ return ((this->type == POP || this->type == SCAN || this->type == MATCH_TOKEN) && this->type == a.type) || (this->type == a.type && this->prodLhs == a.prodLhs && this->prodRhsIndex == a.prodRhsIndex); }
    bool operator != (const Action &a) const{ return !(*(this) == a); }
    bool isInvalid(){ return type < 1 || type > 4; }

    string toString()
    { 
        if(isInvalid()) return "A(INVALID)";

        string ans("A("); 
        if(type == Action::POP) ans += "POP";
        else if(type == Action::SCAN) ans += "SCAN";
        else if(type == Action::MATCH_TOKEN) ans += "MATCH_TOKEN";
        else {
            ans += "PRODUCE["; 
            if(!Action::prodMap) ans += prodLhs.toString() + "," + to_string(prodRhsIndex);
            else{ ans += " " + prodLhs.toString() + " -> "; for(Item i: (*Action::prodMap)[prodLhs][prodRhsIndex]) ans += i.toString() + " "; }
            ans += "]";
        } 
        ans += ")"; 
        return ans; 
    }

    friend ostream & operator << (ostream &os, Action &a);
};

Map *Action::prodMap = nullptr;

ostream & operator << (ostream &os, Action &a)
{
    if(a.isInvalid()){ os << "A(INVALID)"; return os; }

    os << "A(";
    if(a.type == Action::POP) os << "POP";
    else if(a.type == Action::SCAN) os << "SCAN";
    else if(a.type == Action::MATCH_TOKEN) os << "MATCH_TOKEN";
    else {
        os << "PRODUCE["; 
        if(!Action::prodMap) os << a.prodLhs << "," << a.prodRhsIndex;
        else{ os << " " << a.prodLhs << " -> "; for(Item i: (*Action::prodMap)[a.prodLhs][a.prodRhsIndex]) os << i << " "; }
        os << "]";
    }

    os << ")";
    return os;
}

typedef unordered_map<Item, unordered_map<Item, Action, ItemHasher>, ItemHasher> ParsingTable;


Item makeSymbol(string val){ return Item(val, true); }
Item makeTerminal(string val){ return Item(val, false); }
Action makeProduceAction(Item prodLhs, int prodRhsIndex){ return Action(prodLhs, prodRhsIndex); }



class Token
{
public:
    Item tokenType;
    string tokenVal;

    Token(){ tokenType = Item(); }
    Token(Item tokenType, string tokenVal){ this->tokenType = tokenType; this->tokenVal = tokenVal; }

    friend ostream & operator << (ostream &os, Token &t);
};

ostream & operator << (ostream &os, Token &t)
{
    os << "{" << t.tokenType << ": " << t.tokenVal << "}";
    return os;
}

vector<Token> tokeniseCharacterStreamUsingDfa(string &stream, vector<unordered_map<char, int>> &dfaMap, unordered_map<int, string> finalStates)
{
    vector<Token> ans;
    string lexeme;
    int ptr = 0, curState = 0;  // Starting state is always 0 ...
    char ch;
    while(ptr < stream.size())
    {
        ch = stream[ptr++];
        if(dfaMap[curState].find(ch) != dfaMap[curState].end()) curState = dfaMap[curState][ch], lexeme.push_back(ch);
        else
        {
            if(curState == 0) ans.push_back({makeTerminal("INVALID"), string(1, ch)});
            else ans.push_back({makeTerminal(((finalStates.find(curState) != finalStates.end())? finalStates[curState]: "INVALID")), lexeme}), ptr--;

            lexeme.clear();
            curState = 0;
        }
    }

    if(!lexeme.empty()) ans.push_back({makeTerminal(((finalStates.find(curState) != finalStates.end())? finalStates[curState]: "INVALID")), lexeme});

    return ans;
}

void printInputTokens(vector<Token> &tokenList, ostream &o, bool ignoreSeparators = false)
{
    int cnt = 0;
    for(Token t: tokenList) if(t.tokenType.val != "SEP" || !ignoreSeparators) cnt++;
    o << cnt << endl;
    for(Token t: tokenList) if(t.tokenType.val != "SEP" || !ignoreSeparators) o << t.tokenType.val << " " << ((t.tokenVal == "\n")? "\\n": (t.tokenVal == "\t")? "\\t": string(t.tokenVal)) << endl;
}




class ParsingHistoryEntry
{
public:
    vector<Item> curParsingStack;
    vector<Token> curTokenStream;
    Action a;

    ParsingHistoryEntry(vector<Item> &curParsingStack, vector<Token> &curTokenStream, Action a){ this->curParsingStack = curParsingStack; this->curTokenStream = curTokenStream; this->a = a; }

    friend ostream & operator << (ostream &os, ParsingHistoryEntry &e);
};

ostream & operator << (ostream &os, ParsingHistoryEntry &e)
{
    os << "Parsing Stack: "; bool lol = false; for(int i = e.curParsingStack.size() - 1; i >= 0; i--){ if(lol) os << ", "; lol = true; os << e.curParsingStack[i]; } os << endl;
    os << "Token Stream: "; lol = false; for(Token t: e.curTokenStream){ if(lol) os << ", "; lol = true; os << t; } os << endl;
    os << "Action: " << e.a;
    return os;
}

typedef vector<ParsingHistoryEntry> ParsingHistory;




Item EPSILON_TERMINAL = makeTerminal("E");
Item END_TERMINAL = makeTerminal("$");
Action POP_ACTION(Action::POP);
Action SCAN_ACTION(Action::SCAN);
Action MATCH_TOKEN_ACTION(Action::MATCH_TOKEN);
Action INVALID_ACTION(0);







vector<string> myTokenizer(string &s, char del = ' ')
{
    vector<string> ans;
    stringstream ss(s);
    string word;
    while (!ss.eof()){ getline(ss, word, del); if(!word.empty()) ans.push_back(word); word.clear(); }
        
    return ans;
}

pair<Map, pair<Set, Set>> parseProductionsfromInput(vector<string> &productions)
{
    Map prodMap;
    vector<vector<string>> curItemList;
    vector<string> curItems;
    unordered_set<string> symbols;
    Set symbolItems;
    Set terminalItems;

    for(string production: productions)
    {
        curItemList.push_back(myTokenizer(production));
        symbols.insert(curItemList.back()[0]);
        symbolItems.insert(makeSymbol(curItemList.back()[0]));
    }

    int j = 0;
    for(string production: productions)
    {
        curItems = curItemList[j++];
        if(curItems[1] != string("->")){ cout << "Invalid Production: " << production << endl; exit(0); }

        vector<Item> prodRhs;
        for(int i = 2; i < curItems.size(); i++) if(symbols.find(curItems[i]) != symbols.end()) prodRhs.push_back(makeSymbol(curItems[i])); else prodRhs.push_back(makeTerminal(curItems[i])), terminalItems.insert(makeTerminal(curItems[i]));
        prodMap[makeSymbol(curItems[0])].push_back(prodRhs);
    }

    terminalItems.insert(END_TERMINAL);

    return {prodMap, {symbolItems, terminalItems}};
}




pair<vector<string>, Item> readProductionsAsString(fstream &f)
{
    int n; string start;
    f >> n; f >> start;
    Item startSymbol = makeSymbol(start);
    char buffer[100];
    vector<string> ans;

    // cin.clear();
    // fflush(stdin);

    f.getline(buffer, 100);  // flushing newline ...

    while(n--)
    {
        f.getline(buffer, 100);
        ans.push_back(buffer);
    }

    // for(string s: ans) cout << s << endl;

    return {ans, startSymbol};
}




void printProductionList(Map &prodMap, Set &symbolSet, Set &terminalSet, fstream &f)
{
    // for(Map::iterator it = prodMap.begin(); it != prodMap.end(); it++)
    // {
        
    // }
    for (auto prod : prodMap)
    {
        Item leftSymbol = prod.first;
        f << leftSymbol << " -> ";

        bool lol = false;
        for(vector<Item> iList: prod.second)
        {
            if(lol) f << "| ";
            lol = true;

            for(Item i: iList) f << i << " ";
        }

        f << endl;
    }

    f << endl;
    f << "SYMBOLS:  "; bool lol = false; for(Item symbol: symbolSet){ if(lol) f << " , "; lol = true; f << symbol; } f << endl;
    f << "TERMINALS:  "; lol = false; for(Item terminal: terminalSet){ if(lol) f << " , "; lol = true; f << terminal; } f << endl;
}




pair<Map, Set> removeImmediateLeftRecursion(Map &prodMap)
{
    // A → Aα1|Aα2| … . |Aαm|β1|β2| … . . βn
    // can be replaced by -->
    // A → β1A'|β2A'| … . . | … . . |βnA'
    // A → α1A'|α2A'| … . . |αmA'|ε

    Map prodMapAns;
    Set extraSymbols;
    for(auto prod : prodMap)
    {
        Item leftSymbol = prod.first;
        vector<vector<Item>> rightList = prod.second;

        vector<vector<Item>> hasLr, noLr;
        for(vector<Item> iList: rightList) if(iList.front() == leftSymbol) hasLr.push_back(iList); else noLr.push_back(iList);

        if(hasLr.empty()) prodMapAns[leftSymbol] = rightList;
        else
        {
            // S -> S alpha | beta

            // S -> beta S'
            // S' -> alpha S' | E
            Item augmentedLeftSymbol = makeSymbol(leftSymbol.val + "'");
            extraSymbols.insert(augmentedLeftSymbol);
            vector<vector<Item>> alphaList, betaList;

            for(vector<Item> lol: hasLr)
            {
                vector<Item> v;
                for(int i = 1; i < lol.size(); i++) v.push_back(lol[i]);
                alphaList.push_back(v);
            }

            for(vector<Item> lol: noLr)
            {
                vector<Item> v;
                for(int i = 0; i < lol.size(); i++) v.push_back(lol[i]);
                betaList.push_back(v);
            }

            for(vector<Item> lol: betaList)
            {
                vector<Item> v;
                for(int i = 0; i < lol.size(); i++) v.push_back(lol[i]);
                v.push_back(augmentedLeftSymbol);
                prodMapAns[leftSymbol].push_back(v);
            }

            for(vector<Item> lol: alphaList)
            {
                vector<Item> v;
                for(int i = 0; i < lol.size(); i++) v.push_back(lol[i]);
                v.push_back(augmentedLeftSymbol);
                prodMapAns[augmentedLeftSymbol].push_back(v);
            }

            prodMapAns[augmentedLeftSymbol].push_back({makeTerminal("E")});
        }
    }

    return {prodMapAns, extraSymbols};
}




MapNullable generateNullableMap(Map &prodMap, Set &symbols)
{
    MapNullable mapNullable;

    Set remainingSymbols;
    for(Item i: symbols) if(find(prodMap[i].begin(), prodMap[i].end(), vector<Item>({EPSILON_TERMINAL})) != prodMap[i].end()) mapNullable[i] = true;
                         else remainingSymbols.insert(i);

    bool updated = true;
    while(updated && !remainingSymbols.empty())
    {
        updated = false;

        for(Item symbol: remainingSymbols)
        {
            bool nullableFound = false;
            for(vector<Item> ItemList: prodMap[symbol])
            {
                bool notNullableItemFoundInProd = false;
                for(Item i: ItemList) if((i.isSymbol && !mapNullable[i]) || !i.isSymbol){ notNullableItemFoundInProd = true; break; }

                if(!notNullableItemFoundInProd){ nullableFound = true; break; }
            }

            if(nullableFound) updated = true, mapNullable[symbol] = true, remainingSymbols.erase(symbol);
        }
    }

    for(Item symbol: remainingSymbols) mapNullable[symbol] = false;

    return mapNullable;
}

bool tryInsertHelper1(Item i, Set &s)
{
    if(s.find(i) != s.end()) return false;
    else s.insert(i); return true;
}

MapFirst generateFirstFromProductions(Map &prodMap, Set &symbols, MapNullable &mapNullable)
{
    MapFirst mapFirst;
    Item firstElem;

    // Flooding with immediate non-terminals ...
    for(Item symbol: symbols)
    {
        for(vector<Item> itemList: prodMap[symbol])
        {
            firstElem = itemList.front();
            if(!firstElem.isSymbol && firstElem != EPSILON_TERMINAL) mapFirst[symbol].insert(firstElem);
        }
    }

    // Solving left recursive cases with updates done ...
    // We know immediate and non-immediate left recursions have been removed here ...
    bool updated = true;
    while(updated)
    {
        updated = false;

        for(Item symbol: symbols)
        {
            for(vector<Item> itemList: prodMap[symbol])
            {
                if(itemList.front() == EPSILON_TERMINAL) continue;

                for(Item i: itemList)
                {
                    if(i.isSymbol){ for(Item fi: mapFirst[i]) if(tryInsertHelper1(fi, mapFirst[symbol])) updated = true; if(!mapNullable[i]) break; }
                    else{ if(tryInsertHelper1(i, mapFirst[symbol])) updated = true; break;  /* non-terminal is non-nullable ... */  }
                }
            }
        }
    }

    return mapFirst;
}




Set deduceFirstHelper(int pos, vector<Item> &prod, MapNullable &mapNullable, MapFirst &mapFirst)
{
    Set curFirstSet;
    Item i;

    for(; pos < prod.size(); pos++)
    {
        i = prod[pos];

        if(i.isSymbol){ for(Item fi: mapFirst[i]) curFirstSet.insert(fi); if(!mapNullable[i]) break; }
        else{ curFirstSet.insert(i); break;  /* non-terminal is non-nullable ... */  }
    }

    return curFirstSet;
}

MapFollow generateFollowFromProductions(Map &prodMap, Set &symbols, MapNullable &mapNullable, MapFirst &mapFirst, Item startSymbol)
{
    MapFollow mapFollow;
    Item i;

    // First step ...
    mapFollow[startSymbol].insert(END_TERMINAL);

    // Two rules --> 
    // (1) For each production of the form A -> αBβ, add the first set of β to the follow set of B, except for ε.
    // (2) For each production of the form A -> αB, add the follow set of A to the follow set of B.

    for(Item symbol: symbols)
    {
        for(vector<Item> itemList: prodMap[symbol])
        {
            if(itemList.front() == EPSILON_TERMINAL) continue;

            for(int j = 0; j < itemList.size(); j++)
            {
                i = itemList[j];
                if(!i.isSymbol) continue;
                
                // First Type ...
                if(j != itemList.size() - 1){ for(Item f: deduceFirstHelper(j + 1, itemList, mapNullable, mapFirst)) tryInsertHelper1(f, mapFollow[i]);
                // cout << symbol << " -> pos = " << j << " | Item = " << i << " | First_of_Next = "; for(Item f: deduceFirstHelper(j + 1, itemList, mapNullable, mapFirst)) cout << f << ", "; cout << endl;
                // cout << "Actual Prod =>  " << symbol << " -> "; for(Item a: itemList) cout << a << ", "; cout << endl;
                // cout << "FOLLOW --> " << endl;
                // for(auto lol: mapFollow){ Item i = lol.first; cout << i << " -> { "; bool b = false; for(Item a: lol.second){ if(b) cout << ", "; b = true; cout << a << " "; } cout << "}" << endl; } 
                // cout << endl;
                }
            }
        }
    }

    // cout << endl << "Rule 2 -->" << endl << endl;

    bool updated = true; int cnt = 1;
    while(updated)
    {
        updated = false;

        for(Item symbol: symbols)
        {
            for(vector<Item> itemList: prodMap[symbol])
            {
                if(itemList.front() == EPSILON_TERMINAL) continue;

                for(int j = itemList.size() - 1; j >= 0; j--)
                {
                    // Second Type ...
                    i = itemList[j];
                    if(i.isSymbol){ for(Item f: mapFollow[symbol]) if(tryInsertHelper1(f, mapFollow[i])) updated = true; if(!mapNullable[i]) break; }
                    else break;  /* non-terminal is non-nullable ... */
                }
            }
        }
    }

    // for(Item symbol: symbols)
    // {
    //     for(vector<Item> itemList: prodMap[symbol])
    //     {
    //         if(itemList.front() == EPSILON_TERMINAL) continue;
    //         for(int j = 0; j < itemList.size(); j++)
    //         {
    //             i = itemList[j];
    //             if(!i.isSymbol) continue;      
    //             if(j != itemList.size() - 1)
    //             {
    //                 // First Type ...
    //                 for(Item f: deduceFirstHelper(j + 1, itemList, mapNullable, mapFirst)) if(tryInsertHelper1(f, mapFollow[i])) updated = true;
    //             }
    //             else
    //             {
    //                 // Second Type ...
    //                 for(Item f: mapFollow[symbol]) if(tryInsertHelper1(f, mapFollow[i])) updated = true;
    //             }
    //         }
    //     }
    // }

    return mapFollow;
}




bool deduceNullableHelper(vector<Item> &prod, MapNullable &mapNullable)
{
    for(Item i: prod)
        if(i.isSymbol){ if(!mapNullable[i]) return false; }
        else return false;  /* non-terminal is non-nullable ... */

    return true;
}

bool tryInsertHelper2(Item symbol, Item terminal, Action a, ParsingTable &parsingTable)
{
    if(parsingTable[symbol][terminal].isInvalid()){ parsingTable[symbol][terminal] = a; return true; }
    else return parsingTable[symbol][terminal] == a;  // Because of multiple reasons , same production can be inserted ...
}

void tryInsertHelper3(Item symbol, Item terminal, int index, ParsingTable &parsingTable)
{
    Action a = makeProduceAction(symbol, index);
    if(!tryInsertHelper2(symbol, terminal, a, parsingTable)) {
        cout << "AMBIGUOUS GRAMMAR FOUND !!!" << endl;
        cout << "parsingTable[" << symbol << "][" << terminal << "]  -->  " << parsingTable[symbol][terminal] << "  and  " << a << endl;
        exit(0);
    }
}

ParsingTable generateLl1ParsingTable(Map &prodMap, Set &symbols, MapNullable &mapNullable, MapFirst &mapFirst, MapFollow &mapFollow)
{
    // parsingTable[Symbol][Terminal] -> Action ...
    ParsingTable parsingTable;

    for(Item symbol: symbols)
    {
        int index = -1;
        for(vector<Item> itemList: prodMap[symbol])
        {
            index++;
            if(itemList.front() == EPSILON_TERMINAL)
            {
                for(Item terminal: mapFollow[symbol]) tryInsertHelper3(symbol, terminal, index, parsingTable);
                continue;
            }

            for(Item i: itemList)
            {
                for(Item terminal: deduceFirstHelper(0, itemList, mapNullable, mapFirst)) tryInsertHelper3(symbol, terminal, index, parsingTable);
                if(deduceNullableHelper(itemList, mapNullable)) for(Item terminal: mapFollow[symbol]) tryInsertHelper3(symbol, terminal, index, parsingTable);
            }
        }
    }

    return parsingTable;
}

void augmentLl1ParsingTableWithErrorRecovery(Map &prodMap, Set &symbols, Set &terminals, MapNullable &mapNullable, MapFirst &mapFirst, MapFollow &mapFollow, ParsingTable &parsingTable)
{
    for(Item symbol: symbols)
    {
        for(Item terminal: terminals)
        {
            if(!parsingTable[symbol][terminal].isInvalid()) continue;

            if(terminal == END_TERMINAL || mapFollow[symbol].find(terminal) != mapFollow[symbol].end()) parsingTable[symbol][terminal] = POP_ACTION;
            else parsingTable[symbol][terminal] = SCAN_ACTION;
        }
    }
}

void printParsingTable(ParsingTable &parsingTable, Set &symbols, Set &terminals, fstream &f, int COL_WIDTH = 100)
{
    Action a;

    // Heading (of Terminals) ...
    f << setw(COL_WIDTH) << left << " ";  // Left align and min-width setting is only for the next token in the stream ...
    for(Item terminal: terminals) f << setw(COL_WIDTH) << left << terminal.toString();
    f << endl;

    for(Item symbol: symbols)
    {
        // Row (of Symbols) ...
        f << setw(COL_WIDTH) << left << symbol.toString();

        for(Item terminal: terminals)
        {
            a = parsingTable[symbol][terminal];
            // cout << "Lol" << a.isInvalid() << endl;
            f << setw(COL_WIDTH) << left << a.toString();
        }
        f << endl;
    }
}




pair<pair<bool, string>, ParsingHistory> parseTokenStreamUsingParsingTable(vector<Token> &curTokenStream, Set &symbols, Set &terminals, Item startSymbol, ParsingTable &parsingTable, Map &prodMap)
{
    ParsingHistory parsingHistory;
    vector<Item> curParsingStack({END_TERMINAL, startSymbol});
    Action a;
    Item i;
    Token t;
    bool errorFound = false;
    int tokenPos = 0;

    while(tokenPos < curTokenStream.size() || !curParsingStack.empty())
    {
        // Decide Action ...
        i = curParsingStack.back();
        t = curTokenStream[tokenPos];
        if(i.isSymbol) a = parsingTable[i][t.tokenType];
        else
        {
            if(i == t.tokenType) a = MATCH_TOKEN_ACTION;
            else a = INVALID_ACTION;
        }

        // Save History ...
        vector<Token> tempTokenStream({curTokenStream.begin() + tokenPos, curTokenStream.end()});
        parsingHistory.push_back({curParsingStack, tempTokenStream, a});

        // Perform Action ...
        // curParsingStack.pop_back();
        if(a == MATCH_TOKEN_ACTION) tokenPos++, curParsingStack.pop_back();
        else if(a == POP_ACTION) curParsingStack.pop_back(), errorFound = true;
        else if(a == SCAN_ACTION) tokenPos++, errorFound = true;
        else if(a.isInvalid()) return {{true, "Fatal Error"}, parsingHistory};
        else{ curParsingStack.pop_back(); for(int j = prodMap[a.prodLhs][a.prodRhsIndex].size() - 1; j >= 0; j--) if(prodMap[a.prodLhs][a.prodRhsIndex][j] != EPSILON_TERMINAL) curParsingStack.push_back(prodMap[a.prodLhs][a.prodRhsIndex][j]); }
    }

    return {{errorFound, ((errorFound)? "Recovered Error": "No Error")}, parsingHistory};
}

void printParsingHistory(ParsingHistory &parsingHistory, fstream &f){ for(ParsingHistoryEntry e: parsingHistory) f << e << endl << endl; }

vector<Token> readTokenStreamFromFile(fstream &f)
{
    vector<Token> tokenStream;
    int n;
    f >> n;

    string tokenType, tokenVal;
    while(n--)
    {
        f >> tokenType >> tokenVal;
        tokenStream.push_back({makeTerminal(tokenType), tokenVal});
    }

    tokenStream.push_back({END_TERMINAL, "$"});
    return tokenStream;
}

void printReadTokenStream(vector<Token> &tokenStream, fstream &f)
{
    f << "[";
    bool lol = false; for(Token t: tokenStream){ if(lol) f << ", "; lol = true; f << t; }
    f << "]";
}