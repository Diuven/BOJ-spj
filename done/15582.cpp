#include <cstdio>
#include <cassert>
#include <vector>
#include <set>
#include <map>
#include <string>
#include <cstdlib>
#include <cstring>
#include <algorithm>
using namespace std;


int n,m,c,k;
struct point {
    int x,y;
};
point S[105], B[105];
int line[105];

int getdis(point a,point b) {
    int ret = (a.x-b.x) * (a.x-b.x);
    ret += (a.y-b.y) * (a.y-b.y);
    return ret;
}
int main(int agrc, char **agrv){
    /* argv[1] ���� �Է� �������� ���� �̸��� �־����ϴ�. */
    FILE *in = fopen(agrv[1],"r");
    /* argv[2] ���� "��� ���"�� ���� �̸��� �־����ϴ�. */
    FILE *sol = fopen(agrv[2],"r");
    /* argv[3] ���� ������ ���α׷��� ��� ����� ������ �ִ� ���� �̸��� �־����ϴ�. */
    /* ����: �� ������ ���� ���� ��� ������ �ؼ��� �ȵ˴ϴ�.
       ���� ���, ������ "������ �ϴ� Ƚ���� �ּҰ��� ����Ѵ�." ��� �Ǿ� �������� �ұ��ϰ�
       �� ���Ͽ��� ������ �Ǽ�, ������ ���ڿ��� ������ ���� �ִٰ� �����ϰ� ä�� ���α׷��� �ۼ��ؾ� �մϴ�. */
    FILE *out = fopen(agrv[3],"r");

    int ans;
    fscanf(sol,"%d",&ans);

    if(ans == -1) {
        int yourans;
        assert(fscanf(out,"%d",&yourans)==1);
        assert(yourans == -1);
    } else {
        int yourans;
        assert(fscanf(out,"%d",&yourans)==1);
        assert(yourans == ans);

        fscanf(in,"%d%d%d%d",&n,&m,&c,&k);
        for(int i=1;i<=n;i++) {
            fscanf(in,"%d%d",&S[i].x,&S[i].y);
        }
        for(int i=1;i<=m;i++) {
            fscanf(in,"%d%d",&B[i].x,&B[i].y);
        }
        for(int i=1;i<=k;i++) {
            int cnt;
            fscanf(in,"%d",&cnt);
            for(int j=0;j<cnt;j++) {
                int idx;
                fscanf(in,"%d",&idx);
                line[idx] = i;
            }
        }

        int yans[105];
        for(int i=1;i<=n;i++) {
            assert(fscanf(out,"%d",&yans[i])==1);
            assert(1<=yans[i] && yans[i]<=m);
        }
        for(int i=1;i<=n;i++) {
            assert(getdis(S[i] , B[yans[i]]) <= ans);
        }

        int Scnt[105]={0,};
        for(int i=1;i<=n;i++) {
            Scnt[ line[yans[i]] ]++;
        }
        for(int i=1;i<=k;i++) {
            assert(Scnt[i] <= c);
        }

    }
    int b;
    assert(fscanf(out,"%d",&b)==-1);
    fclose(in); fclose(out); fclose(sol);
    return 0;
}
