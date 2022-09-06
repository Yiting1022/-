#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
//1s1s2s2s3s3s5s5s1z1z2z2z8s8s
//p 151~159
//s 181~189
//w 221~229
//z 251~257
char types[36][30]={"斷幺九0","門前清自摸和1","役牌:自風牌2","役牌:場風牌3","役牌:白4","役牌:發5","役牌:中6","一盃口7","平和8","三色同刻9","三色同順10","三槓子11","對對和12","三暗刻13","小三元14","混老頭15","七對子16","混全帶么九17","一氣通貫18","純全帶么九19","混一色20","二盃口21","清一色22","大三元23","四暗刻24","四暗刻單騎25","小四喜26","大四喜27","字一色28","綠一色29","清老頭30","國士無雙31","國士無雙十三面聽32","九蓮寶燈33","純正九蓮寶燈34","四槓子35"};
int yaojo[13]={151,159,181,189,221,229,251,252,253,254,255,256,257};
int green[6]={182,183,184,186,188,256};
char man_[6][30]={"","滿貫","跳滿","倍滿","三倍滿","累計役滿"};
char othertype[7][30]={"立直","雙立直","一發","海底摸月","河底撈魚","搶槓","嶺上開花"};
//settings
//*************************************************************************************************************

int others_[7]={0,0,0,0,0,0,0};//立直/雙立直/一發/海底摸月/河底撈魚/搶槓/嶺上開花
bool chin=0;            //莊家
bool tsumo=0;           //自摸
int dora=0;             //寶牌
int yipai[5]={251,251,255,256,257};//門風/場風/./././


//*************************************************************************************************************

typedef struct returnset_{
    int fu;
    int fan;
    int* yee;
    int y;
    int yakuman;
}RS;
int cmp(const void* a, const void* b) {
    return *(const int*) a - *(const int*) b;
}

RS* calculate(int head,int* hand,int h,int* shzu,int sh,int* gnzu,int gn,int* krzu,int kr,int fu,bool furu,int last,int ankr,bool mode){
    

    int* count = malloc(sizeof(int)*258);
    bool* four = malloc(sizeof(bool)*230);
    int cshzu[8]={0};
    int cgnzu[8]={0};
    int ckrzu[8]={0};
    int zpai[8]={0};
    bool rigu=1;
    bool rigu2=0;
    bool guo=1;
    int jolian=0;
    int good=0;

    for(int i=0; i<h; i++)
        count[hand[i]]++;
    for(int i=0; i<sh; i++)
        cshzu[i]=shzu[i];
    for(int i=0; i<kr; i++)
        ckrzu[i]=krzu[i];
    for(int i=0; i<gn; i++)
        cgnzu[i]=gnzu[i];
    for(int i=0; i<h; i++){
        if(count[hand[i]]!=2)
            rigu=0;
    }
    if(h!=14)
        rigu=0;
    for(int i=151; i<=229; i++){
        if(count[i]==4)
            four[i]=1;
    }
    
    for(int i=0; i<13; i++){
        if(yaojo[i]!=head && count[yaojo[i]]!=1)
            guo=0;
        else if(yaojo[i]==head && count[yaojo[i]]!=2)
            guo=0;
    }
    if(h==14 && head<250){
        jolian=1;
        int adder=head-head%10;
        for(int i=1; i<=9; i++){
            if(i==1 || i==9){
                if(count[i+adder]!=3 && count[i+adder]!=4){
                    jolian=0;
                    break;
                }
            }else{
                if(count[i+adder]!=2 && count[i+adder]!=1){
                    jolian=0;
                    break;
                }
            }
        }
        if((count[last]==2 ||count[last]==4)&&jolian)
            jolian=2;
    }
    count[head]-=2;
    for(int i=151; i<=229; i++){
        if(count[i]<0){
            rigu2=1;
            if(!rigu && !guo){
                return NULL;
            }else break;
        }
        if(count[i]==1){
            if(i%10>=8 && !rigu && !guo){
                return NULL;
            }
            cshzu[sh++]=i;
            count[i]--;
            count[i+1]--;
            count[i+2]--;
            if(last==i || last==i+2){
                if(!(last==i+2 && i%10==1)&&!(last==i && i%10==7)&&good!=-1)
                    good=1;
                else good=-1;
            }
        }else if(count[i]==2){
            if(i%10>=8 && !rigu && !guo){
                return NULL;
            }
            cshzu[sh++]=i;
            cshzu[sh++]=i;
            count[i]-=2;
            count[i+1]-=2;
            count[i+2]-=2;
            if(last==i || last==i+2){
                if(!(last==i+2 && i%10==1)&&!(last==i && i%10==7)&&good!=-1)
                    good=1;
                else good=-1;
            }
        }else if(count[i]==3){
            if(mode==0){
                count[i]-=3;
                ckrzu[kr++]=i;
                if(last!=i || tsumo || four[i]){
                    ankr++;
                    if(i%10==9||i%10==1||i>250)
                        fu+=8;
                    else fu+=4;
                }else{
                   if(i%10==9||i%10==1||i>250)
                       fu+=4;
                   else fu+=2;
                }
                if(last==i)
                    good=1;
            }else{
                if(count[i+1]!=3 || count[i+2]!=3)
                    return NULL;
                count[i]-=3;
                count[i+1]-=3;
                count[i+2]-=3;
                cshzu[sh++]=i;
                cshzu[sh++]=i;
                cshzu[sh++]=i;
                if(last==i || last==i+2){
                    if(!(last==i+2 && i%10==1)&&!(last==i && i%10==7)&&good!=-1)
                        good=1;
                    else good=-1;
                }
                
            }
        }else if(count[i]==4){
            if(i%10>=8)
                return NULL;
            count[i]-=4;
            count[i+1]--;
            count[i+2]--;
            cshzu[sh++]=i;
            ckrzu[kr++]=i;
            ankr++;
            if(i%10==1||i>250)
                fu+=8;
            else fu+=4;
            if(last==i)
                good=1;
        }
    }
    for(int i=251; i<=257; i++){
        if(count[i]==3){
            ckrzu[kr++]=i;
            count[i]-=3;
            if(last!=i || tsumo){
                fu+=8;
            }else fu+=4;
            if(last==i)
                good=1;
        }else if(count[i]==2 && rigu){
            rigu2=1;
            break;
        }
    }
    if(head==last)
        good=0;
    if(good!=1){
        fu+=2;
    }
    if(tsumo)
        fu+=2;
    else if(!furu && !tsumo)
        fu+=10;
    for(int i=0; i<5; i++){
        if(yipai[i]==head)
            fu+=2;
    }
    RS* ret=malloc(sizeof(RS));
    ret->yee=malloc(sizeof(int)*30);
    ret->fan=0;
    ret->y=0;
    ret->yakuman=0;
    ret->fu=fu+20;
    bool flag_p=0;
    bool flag_s=0;
    bool flag_w=0;
    bool flag_z=0;
    bool flag_0=1;
    bool flag_30=1;
    bool flag_15=1;
    bool flag_17=1;
    bool flag_19=1;
    bool flag_28=1;
    bool flag_29=0;

    if(tsumo && !furu){
        ret->fan++;
        ret->yee[ret->y++]=1;//門清自摸
    }
    if(guo){
        ret->yakuman=1;
        if(head==last){
            ret->fan+=26;
            ret->yee[ret->y++]=32;//國士無雙十三面聽
            ret->fu=20;
        }else{
            ret->fan+=13;
            ret->yee[ret->y++]=31;//國士無雙
            ret->fu=20;
        }
        return ret;
    }
    if(jolian>0){
        if(jolian==2){
            ret->yakuman=2;
            ret->fan+=26;
            ret->yee[ret->y++]=34;//純正九蓮寶燈
        }else{
            ret->yakuman=1;
            ret->fan+=13;
            ret->yee[ret->y++]=33;//九蓮寶燈
        }
        //return ret;
    }
    if(rigu&&rigu2){
        sh=0;
        ret->fan+=2;
        ret->yee[ret->y++]=16;//七對子
        ret->fu=25;
    }
    if(head>250){
        flag_30=0;
        flag_0=0;
        flag_19=0;
        flag_z=1;
        zpai[head-250]+=2;
    }else if(head%10!=9 && head%10!=1){
        flag_30=0;
        flag_15=0;
        flag_17=0;
        flag_19=0;
        flag_28=0;
    }else{
        flag_0=0;
        flag_28=0;
    }
    if(head<160)
        flag_p=1;
    else if(head>160 && head<220)
        flag_s=1;
    else if(head>220 && head<250)
        flag_w=1;
    
    if(head<250)
        flag_28=0;

    for(int i=0; i<h; i++){
        if(hand[i]>250){
            flag_30=0;
            flag_19=0;
            flag_0=0;
            flag_z=1;
        }
        else if(hand[i]%10!=9 && hand[i]%10!=1){
            flag_30=0;
            flag_15=0;
            flag_28=0;
        }else{
            flag_0=0;
            flag_28=0;
        }
        if(hand[i]<160)
            flag_p=1;
        if(hand[i]>160 && hand[i]<220)
            flag_s=1;
        if(hand[i]>220 && hand[i]<250)
            flag_w=1;
        if(hand[i]<250)
            flag_28=0;
    }
    for(int i=0; i<gn; i++){
        if(cgnzu[i]>250){
            flag_30=0;
            flag_19=0;
            flag_0=0;
            flag_z=1;
            zpai[cgnzu[i]-250]+=3;
        }
        else if(cgnzu[i]%10!=9 && cgnzu[i]%10!=1){
            flag_30=0;
            flag_15=0;
            flag_17=0;
            flag_19=0;
            flag_28=0;
        }else{
            flag_0=0;
            flag_28=0;
        }
        if(cgnzu[i]<160)
            flag_p=1;
        if(cgnzu[i]>160 && cgnzu[i]<220)
            flag_s=1;
        if(cgnzu[i]>220 && cgnzu[i]<250)
            flag_w=1;
    }
    for(int i=0; i<kr; i++){
        if(ckrzu[i]>250){
            flag_30=0;
            flag_19=0;
            flag_0=0;
            flag_z=1;
            zpai[ckrzu[i]-250]+=3;
        }
        else if(ckrzu[i]%10!=9 && ckrzu[i]%10!=1){
            flag_30=0;
            flag_15=0;
            flag_17=0;
            flag_19=0;
        }else{
            flag_0=0;
            flag_28=0;
        }
        if(ckrzu[i]<160)
            flag_p=1;
        if(ckrzu[i]>160 && ckrzu[i]<220)
            flag_s=1;
        if(ckrzu[i]>220 && ckrzu[i]<250)
            flag_w=1;
    }
    for(int i=0; i<sh; i++){
        if(cshzu[i]%10!=1 && cshzu[i]%10!=7){
            flag_17=0;
            flag_19=0;
        }
        else flag_0=0;
        if(cshzu[i]<160)
            flag_p=1;
        if(cshzu[i]>160 && cshzu[i]<220)
            flag_s=1;
        if(cshzu[i]>220 && cshzu[i]<250)
            flag_w=1;
    }
    if(sh!=0){
        flag_15=0;
        flag_30=0;
    }else{
        flag_19=0;
        flag_17=0;
    }
    if(flag_28){
        ret->fan+=13;
        ret->yee[ret->y++]=28;//字一色
        ret->yakuman++;
    }else if(flag_30){
        ret->fan+=13;
        ret->yee[ret->y++]=30;//清老頭
        ret->yakuman++;
    }else if(flag_15){
        ret->fan+=2;
        ret->yee[ret->y++]=15;//混老頭
    }else if(flag_19){
        ret->yee[ret->y++]=19;//純全帶么九
        if(furu)
            ret->fan+=2;
        else ret->fan+=3;
    }else if(flag_17){
        ret->yee[ret->y++]=17;//混全帶
        if(furu)
            ret->fan+=1;
        else ret->fan+=2;
    }else if(flag_0){
        ret->yee[ret->y++]=0;//斷幺九
        ret->fan+=1;
    }
    if(flag_s+flag_w+flag_p==1){
        if(flag_z){
            ret->yee[ret->y++]=20;//混一色
            if(furu)
                ret->fan+=2;
            else
                ret->fan+=3;
        }else{
            ret->yee[ret->y++]=22;//清一色
            if(furu)
                ret->fan+=5;
            else
                ret->fan+=6;
        }
    }
    if(zpai[5]+zpai[6]+zpai[7]==9){
        ret->yakuman++;
        ret->fan+=13;
        ret->yee[ret->y++]=23;//大三元
    }else if(zpai[5]+zpai[6]+zpai[7]==8){
        ret->fan+=2;
        ret->yee[ret->y++]=14;//小三元
    }else if(zpai[1]+zpai[2]+zpai[3]+zpai[4]==12){
        ret->yakuman+=2;
        ret->fan+=26;
        ret->yee[ret->y++]=27;//大四喜
    }else if(zpai[1]+zpai[2]+zpai[3]+zpai[4]==11){
        ret->yakuman++;
        ret->fan+=13;
        ret->yee[ret->y++]=26;//小四喜
    }
    for(int i=0; i<5; i++){
        if(zpai[yipai[i]-250]==3){
            ret->fan++;
            ret->yee[ret->y++]=2+i;//役牌
        }
    }
    
    if(gn==4){
        ret->yakuman++;
        ret->fan+=13;
        ret->yee[ret->y++]=35;//四槓子
    }
    for(int i=0; i<6; i++){
        if(head==green[i])
            flag_29=1;
    }
    if(flag_29){
        for(int i=0; i<h; i++){
            if(flag_29){
                flag_29=0;
                for(int j=0; j<6; j++)
                    if(hand[i]==green[j])
                        flag_29=1;
            }
        }
        for(int i=0; i<sh; i++){
            if(cshzu[i]!=182)
                flag_29=0;
        }
        for(int i=0; i<gn; i++){
            if(flag_29){
                flag_29=0;
                for(int j=0; j<6; j++)
                    if(cgnzu[i]==green[j])
                        flag_29=1;
            }else break;
        }
        for(int i=0; i<kr; i++){
            if(flag_29){
                flag_29=0;
                for(int j=0; j<6; j++)
                    if(ckrzu[i]==green[j])
                        flag_29=1;
            }else break;
        }
    }
    if(flag_29){
        ret->yee[ret->y++]=29;//綠一色
        ret->fan+=13;
        ret->yakuman++;
    }
    if(kr+gn==4){
        if(!furu)
            if(last == head){
                ret->yakuman+=2;
                ret->fan+=26;
                ret->yee[ret->y++]=25;//四暗刻單騎
            }else if(tsumo){
                ret->yakuman++;
                ret->fan+=13;
                ret->yee[ret->y++]=24;//四暗刻
            }else{
                ret->fan+=4;
                ret->yee[ret->y++]=13;//三暗刻
                ret->yee[ret->y++]=12;//對對和
        }else{
            ret->yee[ret->y++]=12;//對對和
            ret->fan+=2;
            if(ankr==3){
                ret->yee[ret->y++]=13;
                ret->fan+=2;//三暗刻
            }
        }
    }
    else if(ankr==3){
        ret->yee[ret->y++]=13;
        ret->fan+=2;//三暗刻
    }else if(sh>=3){
        for(int i=0; i<sh; i++){
            int k=0;
            for(int j=0; j<sh; j++){
                if(cshzu[j]==cshzu[i]+3)
                    k++;
                else if(cshzu[j]==cshzu[i]-3)
                    k+=2;
            }
            if(k==3){
                if(furu)
                    ret->fan+=1;
                else ret->fan+=2;
                ret->yee[ret->y++]=18;//一氣通貫
                break;
            }
        }
        for(int i=0; i<sh; i++){
            int k=0;
            for(int j=0; j<sh; j++){
                if(cshzu[j]==cshzu[i]+30)
                    k++;
                else if(cshzu[j]==cshzu[i]+70)
                    k+=2;
                 }
            if(k>=3){
                if(furu)
                    ret->fan+=1;
                else ret->fan+=2;
                ret->yee[ret->y++]=10;//三色同順
                break;
            }
        }
    }
    if(gn+kr>=3){
        int tk=0;
        int tonkr[4]={0};
        for(int i=0; i<kr; i++)
            if(ckrzu[i]<250)
                tonkr[tk++]=ckrzu[i];
        for(int i=0; i<gn; i++)
            if(cgnzu[i]<250)
                tonkr[tk++]=cgnzu[i];
        if(tk>=3){
            for(int i=0; i<tk; i++){
                int k=0;
                for(int j=0; j<tk; j++){
                    if(tonkr[j]==tonkr[i]+30)
                        k++;
                    else if(tonkr[j]==tonkr[i]+70)
                        k+=2;
                     }
                if(k==3){
                    ret->fan+=2;
                    ret->yee[ret->y++]=9;//三色同刻
                    break;
                }
            }
        }
    }
    if(sh==4 && !furu){
        bool flag_8 = 0;
        for(int i=0; i<sh; i++){
            if(cshzu[i]==last-2 || cshzu[i]==last)
                if(!(cshzu[i]%10==7&&last==cshzu[i])&&!(cshzu[i]%10==1&&last==cshzu[i]+2))
                    flag_8=1;
        }
        for(int i=0; i<5; i++){
            if(yipai[i]==head)
                flag_8=0;
        }
        if(flag_8){
            ret->fan++;
            ret->yee[ret->y++]=8;//平和
            if(tsumo)
                ret->fu=20;
            else ret->fu=30;
            
        }
    }
    if(!furu && sh>=2){
        if(sh==4 && cshzu[0]==cshzu[1]&&cshzu[2]==cshzu[3]){
            ret->yee[ret->y++]=21;//二盃口
            ret->fan+=3;
        }else{
            for(int i=1; i<sh; i++){
                if(cshzu[i]==cshzu[i-1]){
                    ret->yee[ret->y++]=7;//一盃口
                    ret->fan++;
                    break;
                }
            }
        }
    }
    if(gn==3){
        ret->yee[ret->y++]=11;//三槓子
        ret->fan+=2;
    }
    
    if(furu && ret->fu==20)
        fu=30;
    if(ret->fu%10!=5 && ret->fu%10!=0)
        ret->fu=10*(ret->fu/10)+10;

    
    if(sh+kr+gn>4)
        return NULL;
    return ret;
}
















int main(){
//settings
//*************************************************************************************************************
                                         //自摸
  //  bool chin=1;                                           //莊家
    //bool sanma=0;                                        //三麻 敬請期待
    //yipai[0]=251;                                          //門風
    //yipai[1]=251;                                          //場風
    //int dora=0;                                            //寶牌
//*************************************************************************************************************
    int input;
    
    printf("門風 東:0 南:1 西:2 北:3\n");
    scanf("%d",&input);
    yipai[1]=input+251;
    if(input==0)
        chin=1;
    
    printf("場風 東:0 南:1 西:2 北:3\n");
    scanf("%d",&input);
    yipai[0]=input+251;
    
    printf("榮和:0 自摸:1\n");
    scanf("%d",&input);
    tsumo=input;
 
    printf("立直: 雙立直: 一發: 海底摸月: 河底撈魚: 搶槓: 嶺上開花: y:1/n:0\n");
    for(int i=0; i<7; i++){
        scanf("%d",&input);
        others_[i]=input;
    }
    if(others_[1]==1){
        others_[1]=2;
        others_[0]=0;
    }
    
    printf("寶牌+赤寶牌+裡寶牌:\n");
    scanf("%d",&input);
    dora=input;
    
    int* hand = malloc(sizeof(int)*20);
    int* krzu = calloc(4,sizeof(int));
    int* gnzu = calloc(4,sizeof(int));
    int* shzu = calloc(4,sizeof(int));
    int h=0,kr=0,sh=0,gn=0,furu=0,fu=0,ankr=0;
    char s[48];
    
    printf("請輸入手牌:\n");
    scanf("%s",s);
    int k=0;
    while(s[k]!='\0'){
        if(isnumber(s[k])){
            int num = s[k]-'0';
            k++;
            hand[h++]=num+10*(s[k]-'a');
        }
        else if(s[k]=='['){
            int num = s[k+1]-'0';
            if(num==9||num==1|| s[k+2]=='z')
                fu+=32;
            else fu+=16;
            gnzu[gn++]= num+10*(s[k+2]-'a');
            k+=9;
            ankr++;
        }
        else if(s[k]=='('){
            furu = 1;
            if(s[k+9]==')'){
                int num = s[k+1]-'0';
                if(num==9||num==1|| s[k+2]=='z')
                    fu+=16;
                else fu+=8;
                gnzu[gn++]= num+10*(s[k+2]-'a');
                k+=9;
            }else{
                if(s[k+1]==s[k+3]){
                    int num = s[k+1]-'0';
                    if(num==9||num==1|| s[k+2]=='z')
                        fu+=4;
                    else fu+=2;
                    krzu[kr++]=num+10*(s[k+2]-'a');
                    k+=7;
                }else{
                    int num = s[k+1]-'0';
                    shzu[sh++]=num+10*(s[k+2]-'a');
                    k+=7;
                }
            }
        }
        k++;
    }
    int last = s[k-2]-'0'+10*(s[k-1]-'a');
    qsort(hand,h,sizeof(int),cmp);
    int max=0,maxfu=0;
    RS* rtemp =malloc(sizeof(RS));
    RS* ret=malloc(sizeof(RS));
    int* count = calloc(258,sizeof(int));
    
    for(int i=0; i<h; i++)
        count[hand[i]]++;
    for(int i=150; i<=257; i++){
        if(count[i]>=2){
            for(int j=0; j<=1; j++){
                rtemp=calculate(i,hand,h,shzu,sh,gnzu,gn,krzu,kr,fu,furu,last,ankr,j);
                if(rtemp){
                    if((rtemp->fan>max) || (rtemp->fan==max && rtemp->fu>maxfu)){
                        memcpy(ret,rtemp,sizeof(RS));
                        max=rtemp->fan;
                        maxfu=rtemp->fu;
                    }
                }
            }
        }
    }
    if(ret->yakuman>0){
        for(int i=0; i<ret->y; i++){
            if(ret->yee[i]>=23)
                printf("%s ", types[ret->yee[i]]);
        }
        if(ret->yakuman>=2)
            printf("%d倍役滿 ",ret->yakuman);
        else
            printf("役滿 ");
        printf("%d符\n",ret->fu);
    }else{
        int afan=0;
        for(int i=0; i<7; i++){
            if(others_[i])
                printf("%s ",othertype[i]);
            afan+=others_[i];
        }
        for(int i=0; i<ret->y; i++)
                printf("%s ", types[ret->yee[i]]);
        ret->fan+=afan;
        if(ret->fan>0){
            printf("寶牌%d ",dora);
            ret->fan+=dora;
        }
        printf("%d番 ",ret->fan);
        printf("%d符\n",ret->fu);
    }
    int man=0;
    int point;
    int point1;
    int dian=ret->fu*(2<<(ret->fan+1));
    if(ret->fan==5 || dian>=2000){
        dian=2000;
        man=1;
    }
    if(ret->fan==6 || ret->fan==7){
        dian=3000;
        man=2;
    }
    else if(ret->fan>=8 && ret->fan<=10){
        dian=4000;
        man=3;
    }
    else if(ret->fan>=11 && ret->fan<=12){
        dian=6000;
        man=4;
    }
    else if(ret->fan>=13){
        dian=8000;
        man=5;
    }
    if(ret->yakuman>0){
        dian=8000*ret->yakuman;
        man=0;
    }
    if(tsumo){
        if(chin){
            point=dian*2;
            if(point%100!=0)
                point=100*(point/100)+100;
            printf("%d ALL %s\n",point,man_[man]);
        }
        else{
            point=dian*2;
            if(point%100!=0)
                point=100*(point/100)+100;
            point1=dian;
            if(point1%100!=0)
                point1=100*(point1/100)+100;
            printf("%d-%d %s\n",point1,point,man_[man]);
        }
    }else{
        if(chin){
            point=dian*6;
            if(point%100!=0)
                point=100*(point/100)+100;
            printf("%d %s\n",point,man_[man]);
        }else{
            point=dian*4;
            if(point%100!=0)
                point=100*(point/100)+100;
            printf("%d %s\n",point,man_[man]);
        }
    }
}
    




/*debug小工具
 
 
 printf("%d\n順子:",ret->fan);
 for(int i=0; i<sh; i++)
     printf("%d ",cshzu[i]);
 printf("\n槓子:");
 for(int i=0; i<gn; i++)
     printf("%d ",cgnzu[i]);
 printf("\n刻子:");
 for(int i=0; i<kr; i++)
     printf("%d ",ckrzu[i]);
 printf("\n役:");
 for(int i=0; i<ret->y; i++)
     printf("%s",types[ret->yee[i]]);
 printf("\n");
 
 
 */

