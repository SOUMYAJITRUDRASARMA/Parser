#include <bits/stdc++.h>
using namespace std;
#define ll long long
#define ui unsigned int
#define fastIO ios_base::sync_with_stdio(false); cin.tie(0); cout.tie(0)
#define MOD 1000000007
#define PI 3.14159265358979323846264338327950288419716939937510582097494459230

typedef pair<int, int> pii;
typedef pair<int, char> pic;
typedef pair<int, long> pil;
typedef pair<int, bool> pib;
typedef pair<char, int> pci;
typedef pair<char, char> pcc;
typedef pair<char, long> pcl;
typedef pair<char, bool> pcb;
typedef pair<long, int> pli;
typedef pair<long, char> plc;
typedef pair<long, long> pll;
typedef pair<long, bool> plb;
typedef pair<bool, int> pbi;
typedef pair<bool, char> pbc;
typedef pair<bool, long> pbl;
typedef pair<bool, bool> pbb;

#define madd(a, b) (((a) + (b)) % MOD)
#define msub(a, b) (((a) - (b) + MOD) % MOD)
#define mmul(a, b) (((a) * (b)) % MOD)
ll mpow(ll a, ll b){ if(!b) return 1; ll e = mpow(a, b / 2); return (((e * e) % MOD) * ((b % 2)? a: 1)) % MOD; }
ll gcd(ll a, ll b){ return (!b)? a: gcd(b, a % b); }
ll lcm(ll a, ll b){ return (a * b) / gcd(a, b); }

pii getUpDownRem(int k, int p, int q, string &s)
{
    // k is number of elements ...
    vector<pii> upDown;
    for(int i = 0; i < k; i++) upDown.push_back({'z' - s[i] + 1, s[i] - 'a'});
    sort(upDown.begin(), upDown.end(), [&](pii p1, pii p2){ return p1.first < p2.first; });
    // for(pii p: upDown) cout << p.first << " " << p.second << " , "; cout << endl;
    int i = 0;
    while(i < k && p >= upDown[i].first) p -= upDown[i].first, i++;
    while(i < k && q >= upDown[i].second) q -= upDown[i].second, i++;
    
    // return (i == k)? ({p, q}): ({-1, -1});
    if(i == k) return {p, q};
    else return {-1, -1};

    // return {1, 2};
}

int binarySearchPos(int p, int q, string &s)
{
    int l = 0, r = s.size(), m;
    pii result, inv(-1, -1);
    while(l < r)
    {
        m = (l + r) / 2, result = getUpDownRem(m, p, q, s);
        if(result == inv) r = m - 1;
        else
        {
            if(l == m)
            {
                if(getUpDownRem(l + 1, p, q, s) == inv) return l;
                else return l + 1;
            }
            else l = m;
        }
    }
    
    return l;
}

int main()
{
    int t = 1;
	cin >> t;  // Comment if 1 test case ...
	vector<int> minUpNext, minDownNext;
	while(t--)
	{
		int n, p, q, i = 0, up, down;
		char ch;
		string s;

		cin >> n >> p >> q >> s;
		
		i = binarySearchPos(p, q, s); 
		pii rem = getUpDownRem(i, p, q, s);
        // cout << "i: " << i << endl;
        // cout << "rem: " << rem.first << " , " << rem.second << endl;
        p = rem.first, q = rem.second;
		for(int j = 0; j < i; j++) s[j] = 'a';
		s[i++] -= q;
		while(i < n && p)
		{
		    up = 'z' - s[i] + 1;
		    if(up <= p) s[i] = 'a', p -= up;
		    i++;
		}
		
		
// 		minUpNext.resize(n), minDownNext.resize(n); minUpNext[n - 1] = minDownNext[n - 1] = INT_MAX;
// 		for(int j = n - 1; j > 0; j--) ch = s[j], up = 'z' - ch + 1, down = ch - 'a', minUpNext[j - 1] = min(minUpNext[j], up), minDownNext[j - 1] = min(minDownNext[j], down);

// 		// for(int e: minUpNext) cout << e << " "; cout << endl;
// 		// for(int e: minDownNext) cout << e << " "; cout << endl;

// 		while(i < n && (p > 0 || q > 0))
// 		{
// 			if(s[i] == 'a'){ i++; continue; }

// 			up = 'z' - s[i] + 1, down = s[i] - 'a';
// 			// cout << i << " " << up << " " << down << " " << p << " " << q << " " << endl;

// 			if(i == n - 1){ if(up <= p || down <= q) s[i] = 'a'; else s[i] -= q; i++; continue; }

// 			if(up <= p && down <= q && (p - up) < minUpNext[i] && q < minDownNext[i] && p >= minUpNext[i]) s[i] = 'a', q -= down;
// 			else if(up <= p) s[i] = 'a', p -= up;
// 			else if(down <= q) s[i] = 'a', q -= down;
// 			else if(q) s[i] -= q, q = 0;

// 			i++;
// 		}

		cout << s << endl;
	}

    return 0;
}
