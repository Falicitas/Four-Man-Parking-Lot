#include<bits/stdc++.h>
#include<windows.h>
using namespace std;

/*******************����*********************/
#define finish 1
//#define Test
/************ȫ�ֳ����Լ�����***************/
typedef pair<int,int> PII;  //ӳ���

const int MAX_N = 105;//���9��ͣ����������0����ڣ�100�ǳ��ڡ�����ֹԽ�磩
const int ENTRANCE = 0,EXIT = 100;  //��ڣ����ڵı��
const int MAX_M = 10000;//����ͣ�����ı���
const int MAX_MM = 205; //��ͼ�ı߽�С�ڵ���200
const int INF = 0x3f3f3f3f;
const double eps = 1e-9;
const int ILLEGAL_NUM = 10000;  //����ά��ͼ������ֵ����10000������=0�����ǺϷ���������

const int dir[4][2] = {-1,0,1,0,0,1,0,-1};  //�ĸ����򣬸�bfs��dfs��ά�����ṩ�̶�����

/**************������***************/

class Ranked_list
{
private:
    int num = 0;    //���۵�����
    int Excellent = 0;
    int Fine = 0;
    int Ordinary = 0;
    int Need_improvement = 0;
    int Disappointed = 0;
    int Total_sum = 0;
    double Average_mark = 0;
    vector<string> Feedback;    //�û������
public:
    int Get_num()
    {
        return num; //���num==0�������߲��ܲ鿴
    }
    void get_in_feedback(int marks,string msg)
    {
        if(msg[0]!='\0'&&msg[0]!='\n'){
            Feedback.push_back(msg);
        }
        Total_sum+=marks;
        num++;
        switch(marks/10)
        {
        case 9: case 10:
            Excellent++;
            break;
        case 8:
            Fine++;
            break;
        case 7:
            Ordinary++;
            break;
        case 6:
            Need_improvement++;
            break;
        default:
            Disappointed++;
        }
        Average_mark = (double) Total_sum / (double) num;
    }
    void Rank_show()
    {
        printf("----------------------------------------\n");
        printf("****************************************\n");
        printf("Excellent:%.2f\n",(double)Excellent / (double) num);
        printf("Fine:%.2f\n",(double)Fine / (double) num);
        printf("Fine:%.2f\n",(double)Fine / (double) num);
        printf("Ordinary:%.2f\n",(double)Ordinary / (double) num);
        printf("Need_improvement:%.2f\n",(double)Need_improvement / (double) num);
        printf("Disappointed:%.2f\n",(double)Disappointed / (double) num);
        printf("Average_mark:%.2f\n",Average_mark);
        printf("Do you want to watch the feedback of customer?1or0\n");
        bool num;
        cin>>num;
        if(num){
            cout<<"�û��������У�\n";
            for(int i=0;i<Feedback.size();i++){
                cout<<Feedback[i]<<"\n";
            }
        }
        printf("----------------------------------------\n");
        printf("****************************************\n");
        Sleep(2000);
    }
};

struct EDGE    //�ߵ���Ϣ������ʽ���鴢��
{
    int v,w;  //eΪ�㣬wΪ�ߵ�Ȩֵ
    string road;    ///�ֵ������֣�Ϊ˾���ṩ��Ϣ
    int next;
};

struct car  //������Ϣ���ں����ƺţ�
{
	string number;
	//string car_brand;
	car(string s1)
	{
	    number = s1;
	}
};

struct site //ͣ�����Ĵ�С�Լ��������꣨��ַ��
{
	int number,x,y,l,r;
	site(int number = 0, int a = 0, int b = 0, int c = 0, int d = 0)
		:number(number), x(a), y(b), l(c), r(d)
	{}
	site(const site&A)
	{
	    number = A.number;
	    x = A.x, y = A.y, l = A.l, r = A.r;
	}
	void show() {
		cout << x << "," << y << "," << l << "," << r << '\n';
	}
	void Change_Location(int x, int y, int l, int r) {
		this->x = x;this->y = y;this->l = l;this->r = r;
	}
};

class Space
{
private:
	int Space_Number;//��λ���
	string Space_Name;//��space_number�����
	string car_number;//ͣ������
	string Begin_string;    //��������ʱ����Ϣ
	bool b=0;//�Ƿ�ռ��
	long long Begin_Time;//����ʱ��
public:
	Space(const Space & A)
	{
	    Space_Number = A.Space_Number;
		car_number = A.car_number;
		Begin_string = A.Begin_string;
		Space_Name = A.Space_Name;
		b = A.b;
		Begin_Time = A.Begin_Time;
	}
	Space(bool b,int Space_Number,string name):b(b), Space_Number(Space_Number)
    {
        Space_Name = name;
    }
	int &Get_Space_Number()
	{
	    return Space_Number;//int��
    }//�õ���λ�ţ����н���λ��׼��
	string &Get_SpaceName()
	{
	    return Space_Name;
    }
    string &Get_car_number()
    {
        return car_number;
    }
	string &Get_Begin_string()
	{
	    return Begin_string;
	}
	bool &Get_State()
	{
	    return b;
	}
	long long &Get_Begin_Time()
	{
	    return Begin_Time;
	}
	void Space_clear()
    {
        car_number = "\0";
        Begin_string = "\0";
        b = 0;
        Begin_Time = 0;
    }
	~Space(){};
};

class Parking_Lot
{
private:
	site s;
	vector<vector<Space> > vec;
	int space_num=0;  //ͣ������λ�ܸ���
	int space_res;  //ʣ�೵λ����
public:
	Parking_Lot(const Parking_Lot & A) {
        s = A.s;
        for(int i=0;i<A.vec.size();i++) vec.push_back(A.vec[i]);
        space_num = A.space_num;
        space_res = A.space_res;
    }
	Parking_Lot(const site &s,const vector<int> pile_num):s(s)
	{   //Ϊdirector���vector������׼����ͬʱ���복λ�ռ䣬����λ����
	    for(int i=0;i<pile_num.size();i++){
            vector<Space> buffff;
            for(int j=0;j<pile_num[i];j++){
                string space_name(10,'\0');//ͣ��λ����
                space_name[0] = 'A' + s.number - 1;
                space_name[1] = '-',space_name[2] = i + '0' + 1,space_name[3] = '-';
                int buf = j,ind = 6;
                while(buf){
                    space_name[ind--] = buf % 10 + '0';
                    buf/=10;
                }
                while(ind!=3){
                    space_name[ind--] = '0';
                }   ///����ܵõ�һ���ַ�������:A-2-010��λ
                ///��˼��A��ͣ�������ڶ��㣬��10�ų�λ
                buffff.push_back(Space(0,space_num++,space_name));
                ///������һ����λ��ͣ������λ��������
            }
            vec.push_back(buffff);
	    }
	    space_res = space_num;  ///��ʼͣ����ʣ��ĸ��������ܳ�λ��
	}
	site &Return_Location() {//����ͣ������ַ������ͳ��ȿ�ȣ�
        return s;
    }
    int &Get_num()
    {
        return space_num;
    }
    int &Get_space_res() //����ʣ�೵λ����
    {
        return space_res;
    }
    vector<vector<Space> > &Get_concrete_vec()
    {
        return vec;
    }
    string &space_NName(int PILX,int indx)   ///���س�λ������
    {
        return vec[PILX][indx].Get_SpaceName();
    }
    void Red_space_res()    ///����ʣ�೵λ����
    {
        space_res--;
    }
    void Parking_car(int PILX,int indx,bool b_b,string XXX,long long t)
    {
        vec[PILX][indx].Get_State() = b_b;
        vec[PILX][indx].Get_car_number() = XXX;
        vec[PILX][indx].Get_Begin_Time() = t;
    }

	~Parking_Lot() {};
};

struct Point
{
    int x,y;
    int ind;
    Point(int x=0,int y=0,int ind=0):x(x),y(y),ind(ind){}
};

struct OP///�÷�����ʽǰ������������̻�·��·����Ϣ
{
    int pre=0;
    int x,y;
    OP(int x=0,int y=0,int pre=0):x(x),y(y),pre(pre){}
};

/************ָ����***************/
class Director
{
private:
    ///�������ֳ���ͣ����ϵͳ��������
    int L,R;    //����Ŀ��ռ�ĵ�Ƥ���Ϳ�
    bool is_project = 0;    //��ȷ���˴���ͣ������L��R����������������
    bool is_running = 0;
    double take_charge_rate = 0.2;//Ĭ���շ�ϵ��
    string cipher = "666dlut";
	vector<Parking_Lot> P;//���ʮ��ͣ����,�±�+1=���
	Ranked_list Rank;//���������
	double Final_Income = 0;//ͣ��������
	int total_space = 0;//ͣ������λ���� //ά����ʱ��д��
	int res_space = 0;    //ͣ������λʣ������
	queue<car> Q;   //����
	map<string,PII> car_map;//���泵����ͣ��������Ϣ����ӳ����������ʾ�����š���������ͣ�����������ڼ��㡱���Ż��˳�����ͣ�����ĺ��������㷨
	string Lucky_number = "\0";  //�н�����λ���ƶȷ���
	set<int> colx;//·����ɫ���
	///ͼ������
	site B,E;   //�������ڵĵ�ַ
	EDGE edge[MAX_M]; //����ߵ���Ϣ
	bool flag_dijk = 0; //�ж��Ƿ���Ҫ����dijkstra
	int eid,p[MAX_N];//��ʽǰ���ǵ�ͷָ������
    int dist[MAX_N][2];// �洢��Դ���·�Ľ��
    int new_road[MAX_N][2];//������¼ÿ���������˭�����µģ��Ӵ��ҳ������(����)��ͣ��λ�����·��
    ///��ά��ͼ����
    bool vis_bfs_dfs[MAX_MM][MAX_MM];
    int MAP[MAX_MM][MAX_MM];//����ͣ������ά��ͼ����
    char MAPX[MAX_MM][MAX_MM];
    bool legal_map[MAX_MM][MAX_MM];
    OP op[MAX_MM*MAX_MM];   //bfs·����Ϣ

    //mainmenu w;

public:
    /************����Ŀ¼�������������ߣ�***************/
    ///������ʾ
    void show_main_content();   //������
    void show_manager_content();    //�����߽���
    void show_customer_content();   //�˿ͽ���
    ///ͣ���������복λ����
    void L_R_setting(); //��Ŀ������
    void Space_search_and_insert(string &space_NAME,int &search_num);//�ṩ���һ�·������Ϣ
    bool Legal_Parking_building();//�Ƿ��ܳɹ�����һ��ͣ����
    bool in(int x,int y);//�ж��Ƿ�Խ�����
    string printDate();//�ṩ��ǰʱ����ַ�������
    void add_Space();   /*��δͶ��Ӧ����,��δʵ��*/
    ///�����û�
    void check_in();
    void succeed_in(bool need_to_search,const PII &in_pile,const int &in_space);//ͣ�����Ѿ������ˣ�����Ҫ����dijk��ͬʱ�ⲻ��˾�������ģ�;//�����û�������
    void check_out();
	///ͼ���㷨
    void dijk_init();//ʹ��֮ǰ�ĳ�ʼ��
    void insert1(int u,int v,int w);//һ����uָ��v�������///��in_out����0��ʱ����Ǽ�����ڣ�1Ϊ����
    void insert2(int u,int v,int w);//һ����uָ��v�������///��in_out����0��ʱ����Ǽ�����ڣ�1Ϊ����
    void delete_point(int s);    /*(�䲻����ͼ�ۣ��������ݽṹ�����漰��ά��ͼ�Ĵ���)*/ /*��δͶ��Ӧ����*/
    bool dijkstra_opt(int s,int n,bool in_out);    //s������ڡ����������ͨͼ���򷵻�0����Ϊ�Ƿ���ͣ�������������ж�
    void find_shortest_way(int s,bool in_out);   //���·������
    bool is_road(const Point &A);   //�ж��Ƿ��ǵ�·�������ػ�
    ///��ά��ͼ�����Լ�bfs���·�������㷨
    void MAP_init();//��ͼ��ʼ��
    void Parking_draw();
    void MAP_show();//��ͼչʾ������ͣ�����Լ���·�����۵�չʾ
    void MAP_legal_show();//�Ϸ���ͼչʾ���۲���Щ������Խ���
    void Parking_delete();  /*ɾ����·��dfs*/
    int bfs_opt(site A,site B);
    ///�н�����Ϸϵͳ
    void lottery_system();
    void prime_game_system();
    ///ά��ϵͳ����ʱͣ������ֹӪҵ��
    void safeguard();//ͣ����ά��
    void do_business();//ά��ģʽ��ʱ��Ҫ��ͣ�����ĳ������������ͣ����
    ///�����
    void big_number(int num);   //����Ʊ�ô����ַ�ʽ��ʾ���Ե�����
    void show_Ranked_list();    //չʾͣ�������û�����
    void Customer_Feedback();   //�û��ķ���
    void show_info();   //չʾ�������߻�����Ϣ
    bool TEST(const site &test);    //ͣ���������Բ���
    void find_start_points(const site &A,const site &B,Point &a1);   //�ҵ���ʼ�ķ���
    bool legal_for_road(const Point&A);
    void dye_track(int u,int v,Point &finalx);
    void vis_init();
    void Class_Test();  //������
    void change_charge();   //�޸��շ�ϵ��
    void show_money();  //Ӫҵ�����
    void interface_show();
    void ADD_TRACK();
};

void Director::show_main_content()
{
    cout<<"----------------------------------------\n";
    cout<<"****************************************\n";
    printf("%-7s","�ĸ�����ͣ����APP����\n");
    cout<<"****************************************\n";
    cout<<"----------------------------------------\n";
    cout<<"\n\n";
    cout<<"�������ǹ����߻��ǳ����أ�,����ѡ1��������ѡ0(�˳������밴2):";
    int buf_nu;
    cin>>buf_nu;
    if(buf_nu==2){
        exit(0);
    }
    else if(buf_nu==1){ ///����
        system("cls");
        show_customer_content();
    }
    else{
        system("cls");
        cout<<"����������ߵ����룺";//������ȷ���ܽ���������
        string str;
        int kase = 3;
        while(cin>>str&&str!=cipher&&kase--){
            if(kase>0) cout<<"���������������������(��ʣ"<<kase<<"��)\n";
        }
        if(kase==-1){
            cout<<"����������࣬������������\n";
            Sleep(3000);
            system("cls");
            show_main_content();
            return ;
        }
        cout<<"����������ȷ������������߽���\n";
        Sleep(3000);
        system("cls");
        show_manager_content();
    }
}

void Director::show_manager_content()//��������ߣ����࣬ͬʱҪ�㹻�Զ���
{

    cout<<"���ɲ�����ҳ�棺\n";
    cout<<"A-1:��Ŀ���������ͣ������Ŀ���������������;\n";
    cout<<"A-2:ͣ�����Ľ���;\n";
    /*
    **ͬʱ������ͣ������Ĭ�ϻ����ͣ����ά��ģʽ����ʱ������Ӫҵ
    */
    cout<<"A-3:�ĸ�����ͣ��������Ϣ;\n";//���ظ�������Ŀǰ�ж��ٸ�ͣ������ÿ��ͣ��������Ϣ��ʲô
    cout<<"A-4:��ʼ����Ӫҵ;\n";
    /*����Ҫ��һ��ͣ��������Ӫҵ
    **ͬʱ���������������ڣ���ͼ���Ǹ���ͨͼ����Ӫҵ����ά���ҹ���
    */
    cout<<"A-5:�鿴�ĸ�����ͣ�����Ŀͻ�����;\n";//show_Ranked_list();
    cout<<"A-6:�鿴ͣ�����Ķ�ά��ͼ;\n";//show
    cout<<"A-7:�����ѡ���˳�λ;\n";
    cout<<"A-8:��ǰӪҵ����ʾ;\n";
    cout<<"A-9:�շ�ϵ���޸�;\n";
    cout<<"A-10:����������;\n";
    cout<<"A-11:���԰��;\n";
    cout<<"A-12:�����ĸ�����ͣ�����ı�;\n";
    cout<<"-----------------------------------------------------\n";
    cout<<"������������������(X)��";
    int s1;
    int kase = 3;
    while(cin>>s1&&!(s1>=1&s1<=12)&&kase--){  //��ӹ��ܵ�ʱ��ǵ�����ҲҪ��
        if(kase>=0) cout<<"���������������㻹��"<<kase+1<<"���������:";
    }
    if(kase==-1){
        cout<<"����������࣬����������\n";
        Sleep(3000);
        system("cls");
        show_main_content();
        return ;
    }
    system("cls");
    switch(s1)
    {
    case 1:
        L_R_setting();
        break;
    case 2:
        if(Legal_Parking_building()){
            safeguard();    //���ͣ��������ɹ��������ά��״̬
        }
        break;
    case 3:
        show_info();
        break;
    case 4:
        do_business();
        break;
    case 5:
        show_Ranked_list();
        break;
    case 6:
        MAP_show();
        break;
    case 7:
        lottery_system();
        break;
    case 8:
        show_money();
        break;
    case 9:
        change_charge();
        break;
    case 10:
        cout<<"������������...\n";
        Sleep(3000);
        system("cls");
        show_main_content();

        return ;
    case 11:
        Class_Test();
        break;
    case 12:
        ADD_TRACK();
        break;
    default:
        cout<<"δ֪�����˳�����\n";
        exit(1);
    }
    Sleep(3000);
    system("cls");
    show_manager_content();
}

void Director::show_customer_content()//����ͻ��������ؿͻ�������
{
    cout<<"��ӭʹ���ĸ�����ͣ����~���������\n";
    cout<<"����Ҫ����ʲôҵ���أ�\n";
    cout<<"B-1:�����볡����;\n";
    cout<<"B-2:�����˳�����;\n";  //ͬʱ���ͻ��ṩ�����ʾ�
    cout<<"B-3:�����ĸ����п�����С��Ϸ���л������Ż�ȯŶ��;\n";
    cout<<"B-4:����������;\n";
    cout<<"-----------------------------------------------------\n";
    cout<<"������������������(X)��";
    char s1;
    int kase = 3;
    while(cin>>s1&&!(s1>='1'&&s1<='4')&&kase--){
            if(kase>=0) cout<<"���������������㻹��"<<kase+1<<"���������:";
    }
    if(kase==-1){
        cout<<"����������࣬����������\n";
        show_main_content();
        return ;
    }
    switch(s1-'0')
    {
    case 1:
        system("cls");
        check_in();
        break;
    case 2:
        system("cls");
        check_out();
        break;
    case 3:
        system("cls");
        prime_game_system();
        break;
    case 4:
        cout<<"������������...\n";
        Sleep(3000);
        system("cls");
        show_main_content();
        return;
    default:
        cout<<"δ֪���󣬽��˳�����\n";
        exit(1);
    }
        Sleep(3000);
        system("cls");
    show_customer_content();
}

void Director::L_R_setting()    //����
{
    if(is_project==0){
        cout<<"�������µ��ϰ�����Ҫ�����ˣ���������ΰ���ͼ�������ǽ������˵�ˮƽ���ޣ�ֻ�ܸ��㽨�쳤���120����155��ͣ����Ŷ~�����볤�����";
        cin>>L>>R;
        if(120<=L&&L<=155&&120<=R&&R<=155){
            is_project = 1;
            MAP_init(); //��ͼ��ʼ��
            dijk_init();    //ͼ���㷨��ʼ��
            B = site(0,L-2,4,1,4);
            E = site(100,1,R-5,1,4);
            cout<<"�ɹ���������ع����߽��棩\n";
        }
        else{
            cout<<"��˵�˳����150����200�ˣ���������Ϊ�����ǽ������ˣ�������ʧ�ܣ����ع����߽��棩\n";
        }
    }
    else{
        cout<<"���Ѿ���һ����Ŀ�ˣ����ϰ岻Ҫ̫̰�ģ�(���ع����߽���)\n";
    }
    Sleep(3000);
    system("cls");
    show_manager_content();
}

bool Director::in(int x,int y)//�ж��Ƿ�Խ�����
{
    return 0<=x&&x<L&&0<=y&&y<R;
}

bool Director::TEST(const site &test)//�����ڶ�ά��ͼ���ܷ���Խ�ͣ����
{
    bool flag = 1;
    int l = test.l,r = test.r;
    for(int i=test.x-l;i<=test.x+l;i++){
        for(int j=test.y-r;j<=test.y+r;j++){
            if(in(i,j)){
                if(MAP[i][j]>ILLEGAL_NUM||MAP[i][j]==0){

                }
                else flag = 0;
            }
            else flag = 0;
        }
    }
    /*  ֻ����ĵ����з��յģ���������������ͣ��������ô���ǷǷ�������
    int l = test.l,r = test.r;
    if(in(test.x-l,test.y-r)){
        if(MAP[test.x-l][test.y-r]>ILLEGAL_NUM||MAP[test.x-l][test.y-r]==0){

        }
        else flag = 0;
    }
    else flag = 0;
    if(in(test.x-l,test.y+r)){
        if(MAP[test.x-l][test.y+r]>ILLEGAL_NUM||MAP[test.x-l][test.y+r]==0){

        }
        else flag = 0;
    }
    else flag = 0;

    if(in(test.x+l,test.y-r)){
        if(MAP[test.x+l][test.y-r]>ILLEGAL_NUM||MAP[test.x+l][test.y-r]==0){

        }
        else flag = 0;
    }
    else flag = 0;

    if(in(test.x+l,test.y+r)){
        if(MAP[test.x+l][test.y+r]>ILLEGAL_NUM||MAP[test.x+l][test.y+r]==0){

        }
        else flag = 0;
    }
    else flag = 0;
    */
    return flag;
}

bool Director::Legal_Parking_building()  //����ͣ�����ĺϷ���񣬺Ϸ�����ͣ����
{
    if(!is_project){
        cout<<"����Ƥ��û�У���ô����ͣ������������λ���ϰ����ȥ��(���ع����߽���)\n";
        return false;
    }
    if(P.size()>=9){
        cout<<"ͣ������Ŀ̫������Ŀǰ�ĵ�Ƥ����������ô��ͣ������(���ع����߽���)\n";
        return false;
    }

	cout<<"��ܰ��ʾ������Ȳ鿴һ�µ�ͼ��������Щ�����ʺ��½�ͣ����Ŷ~\n";
    cout<<"(�Ƿ�Ҫ�鿴��ͼ����yes=1,no=0��)��";
    bool flagx;
    cin>>flagx;
    if(flagx){
        MAP_legal_show();
    }
    cout<<"��ʼ����ͣ������������...:\n";
	site test;
	cout<<"����L�ǳ��뾶��R�ǿ�뾶,�����������������Ҹ�����R,�������¸�����L������L��R������6��������3\n";
	cout<<"�������ꡢL��R��x y L R����";
	cin>>test.x>>test.y>>test.l>>test.r;
	test.number = P.size() + 1;
	if(TEST(test)){
        int pile;   //����
        vector<int> pile_num;    //ÿһ�㳵λ����
        cout<<"��ϲ��������������ͣ������\n";
        cout<<"�����������Ҫ���ٲ㣨Ŀǰ�Ľ���ˮƽֻ��������๹�����㣩��";
        int kase = 0;//������
        while(scanf("%d",&pile)==1&&(pile<=0||pile>3)&&kase<=3){
            cout<<"�������ָ����1,2,3������һ�Σ�:";
            kase++;
        }
        if(kase==4){
            cout<<"�������ǲ���������ǽ������˵Ĺ��������ˣ��������ߣ���ͣ��������ʧ�ܣ�\n";
            Sleep(3000);
            system("cls");
            return false;
        }
        for(int i=0;i<pile;i++){
            cout<<"������ҵ�"<<i+1<<"�����ṩ�ĳ�λ(ÿ������3����λ�����20����λ��):";
            int kase = 0;
            int buffff;
            while(scanf("%d",&buffff)==1&&(buffff<3||buffff>20)&&kase<=3){
                cout<<"�ϰ壬����ѧ�ǲ��ǲ��У���������3~20������!:";
                kase++;
            }
            if(kase==4){
                cout<<"�������ǲ���������ǽ������˵Ĺ��������ˣ��������ߣ���ͣ��������ʧ�ܣ�\n";
                Sleep(3000);
                system("cls");
                return false;
            }
            pile_num.push_back(buffff);
        }
        P.push_back(Parking_Lot(test,pile_num));
        cout<<"�ϰ������=w=(ͣ�����ɹ����졣)\n";
        Sleep(3000);
        system("cls");
        return true;
	}
	else {
        cout << "��ͣ���������㽨���������������ڵ��ѽ������ˣ�" << endl;
        Sleep(3000);
        system("cls");
        return false;
    }
}

void Director::safeguard()/*����δʵ��*/
{
    is_running = 0; //ֹͣӪҵ
    ///��ͣ�����ĳ��������ȥ
    cout<<"���ĸ�����ͣ�����Ĺ㲥֪ͨ���ܱ�Ǹ�����ѳ����ǣ���ͣ��������ά�޹���������������Сʱ���뿪�ĸ�����ͣ��������Ϊ��ͻȻ��ά�ޣ����Բ���ͣ�����á�����ɲ��㣬����ԭ�¡�\n\n\n";
    /*������Ҫ����ʲô�ĺ���*/
    car_map.clear();    //�����г������ȥ
    Sleep(7000);
    system("cls");
    total_space = 0;
    for(int i=0;i<P.size();i++){
        P[i].Get_space_res() = 0;   //space_num����ı䣬��resҪ����Ϊspace_num
        for(int j=0;j<P[i].Get_concrete_vec().size();j++){
            for(int k=0;k<P[i].Get_concrete_vec()[j].size();k++){
                P[i].Get_space_res()++;
                total_space++;
                //puts("test1");
                P[i].Get_concrete_vec()[j][k].Space_clear();    //����λ���
            }
        }
    }
    res_space = total_space;
    Parking_draw();//��ͣ�����Լ�Υ��������������
    while(!(dijkstra_opt(0,P.size()+2,0)&&dijkstra_opt(100,P.size()+2,1))){
        cout<<"�������߽�����ʾ����ǰ�ĸ�����ͣ��������һ����ͨͼ������Ӫҵ��\n";
        cout<<"������ĳЩͣ����������ʹ�乹����ͨͼ;\n";
        cout<<"��ܰ��ʾ������Ȳ鿴һ�µ�ͼ������Ҫ���ĸ���������Ŷ~\n";
        cout<<"(�Ƿ�Ҫ�鿴��ͼ(0Ϊ���Խ��죬1Ϊ����)����y=1,n=0��)��";
        bool flagx;
        cin>>flagx;
        if(flagx){
            MAP_legal_show();
        }
        cout<<"��������ͨ�������㣨�����0��������100����u��v����";
        int u,v;
        while(cin>>u>>v&&(u==v||(u>P.size()&&u!=100)||(v>P.size()&&v!=100))){
            printf("u��v������ͬ��Ҫ�Ϸ����������룺");
        }

        site buf1,buf2;
        if(u==0) buf1 = B;
        else if(u==100) buf1 = E;
        else buf1 = P[u-1].Return_Location();
        if(v==0) buf2 = B;
        else if(v==100) buf2 = E;
        else buf2 = P[v-1].Return_Location();
        //printf("test6:%d %d,%d %d\n",buf1.x,buf1.y,buf2.x,buf2.y);
        int w = bfs_opt(buf1,buf2); ///�������·���ĳ��ȣ�������ͼ�еı�Ȩ��
        insert2(u,v,w);
        cout<<"�ɹ���ͨ���أ�\n";
        Sleep(1500);
        system("cls");
    }
    cout<<"�ĸ�����ͣ��������ͨͼ�ѽ��ɣ�\n\n\n";
    Sleep(2000);
}

void Director::show_info()    //�鿴ͣ�����������Ϣ
{
    printf("���ã������ǲ鿴ϵͳ\n");
    if(P.size()!=0){
        printf("��Ŀǰ��%d��ͣ�����������Բ�ѯ1~%d��ͣ������Ϣ������100���ع�����ϵͳ��:",P.size(),P.size());
        int num;
        while(scanf("%d",&num)==1&&num!=100){
            if(num<1||num>P.size()){
                printf("�Ƿ����룡�������������֣�����100Ϊ�˳�����");
            }
            else{
                printf("---------------------------------------\n");
                printf("%-7s%d%s","��",num,"��ͣ��������Ϣ\n");
                printf("***************************************\n");
                printf("%-7s%d%s","����",P[num-1].Get_concrete_vec().size(),"��\n");
                for(int i=0;i<P[num-1].Get_concrete_vec().size();i++){
                    printf("��%d��ĳ�λ����Ϊ��%d\n",i+1,P[num-1].Get_concrete_vec()[i].size());
                }
                printf("\n");
                printf("���룺");
                int flag = 1;
                for(int i=p[num];i!=-1;i=edge[i].next){
                    if(!flag) printf(" ");
                    flag = 0;
                    if(edge[i].v==0) printf("���");
                    else if(edge[i].v==100) printf("����");
                    else printf("%d��ͣ����",edge[i].v);
                }
                printf("����\n");
                printf("***************************************\n");
                printf("---------------------------------------\n");
                printf("�������Բ�ѯ1~%d��ͣ������Ϣ������100���ع�����ϵͳ��:",P.size());
            }
        }
    }
    else{
        printf("������û��һ��ͣ������û����Ϣ�ɲ飡�����ع�����ϵͳ��\n");
    }
    system("cls");
    cout<<"���ڷ���������...\n";
    Sleep(3000);
    system("cls");
    show_manager_content();
    return ;
}

void Director::check_in()//˾������
{
    if(is_running){
        cout<<"��ӭ";
        if(res_space>0){    //����director�����棬���Կ���ֱ�ӵ���
            string s2;
            cout<<"����ڵ�����ʾ����ʾ��������ȴ�����ֱ�ӽ���ͣ������ڰ���������\n";
            cout<<"���ƣ�����Ȼ�����Ƚ����ĸ�����ͣ�����г���ɨ�蹦�ܣ��ٵģ������㲻����������������ĳ��ƺ��복��Ʒ�ƣ�";
            cout<<"���ƣ�";
            cin>>s2;
            Q.push(car(s2));
            succeed_in(1,make_pair(0,0),0);//��������������ʵ������
            return ;
        }
        else{
            cout<<"����ڳ�Ʊ����Ļ��ʾ���ܱ�Ǹ��֪ͨ�㣬��ͣ���������ˣ�\n";
            cout<<"����ڳ�Ʊ����Ļ��ʾ��Ŀǰ��ͣ�����Ŷ�������\n";
            cout<<'\n';
            big_number(Q.size());//���۵Ĵ�����ʾ����˾����ȷ�ķ���
            cout<<'\n';
            cout<<"����ڳ�Ʊ����Ļ��ʾ����Ҫѡ��ȴ�����������ͣ�������ǵ��ĸ�����ͣ�����𣿣�����1or0��\n";
            bool flagxx ;
            cin>>flagxx;
            if(flagxx){
                string s2;
                cout<<"�õġ�����Ȼ�����Ƚ����ĸ�����ͣ�����г���ɨ�蹦�ܣ��ٵģ������㲻����������������ĳ��ƺ��복��Ʒ�ƣ�";
                cout<<"���ƣ�";
                cin>>s2;
                Q.push(car(s2));
                cout<<"����ڳ�Ʊ����Ļ��ʾ��лл�������ģ�������ĳ�Ʊ��(��õ���һ��ֽ��СƱ)\n";
                cout<<"\n\n";
                printf("%-5s\n","�ĸ�����ͣ����");
                printf("%-4s\n","ͣ������");
                printf("------------------------\n");
                printf("���ƺţ�%s\n",s2.c_str());
                printf("ͣ��·�Σ�ͼǿ·\n"
                       "�����غ��Ŵ������ܺ�����Я��������Ʒ��\n"
                       "�������ĸ�����ͣ�����󹤽���������޹�˾\n"
                       "�ͷ�����6666-520dlut\n");
                cout<<"\n\n";
            }
            else{
                cout<<"��л���ĵ������������ʣ��µ�6666-520dlut\n";
            }
        }
    }
    else{
        printf("�ǳ���Ǹ���ĸ�����ͣ������ǰû��Ӫҵ����ӭ�´ι��٣��������ʣ��µ�6666-520dlut\n");
    }
}

/*���·�����������λ�ûд��*/
void Director::succeed_in(bool need_to_search,const PII &in_pile,const int &in_space)//ͣ�����Ѿ������ˣ�����Ҫ����dijk��ͬʱ�ⲻ��˾�������ģ�
{
    cout<<"(�����ʾ��)���ƺţ�"<<Q.front().number<<"�����ѽ���բ��\n";
    string space_NAME;
    int search_num;
    if(need_to_search){
        Space_search_and_insert(space_NAME,search_num);
    }
    else{
        search_num = in_pile.first;
        int i = in_pile.second,j = in_space;
        P[search_num].Parking_car(i,j,1,Q.front().number,time(0));/*֮��һЩ��Ϣ����λ��*/
        P[search_num].Get_concrete_vec()[i][j].Get_Begin_string() = printDate();//����Ʊ�ṩ��Ϣ
        space_NAME = P[search_num].space_NName(i,j);
        P[search_num].Red_space_res();
        res_space--;
        car_map[Q.front().number] = in_pile;
    }
    printf("���ܵ���Ϊ���������ǽ�Ϊ��ѡ�����(·�����)��ͣ��·��\n");
	printf("�����Ƽ���λ��%c��ͣ����,%s�ų�λ\n�Ƽ�·���ǣ�\n", 'A'+search_num, space_NAME.c_str());
	find_shortest_way(search_num+1,0);/*�����·������������*/

    Q.pop();
    Sleep(10000);
}

void Director::check_out()    //��������ﲻΪ�գ���һ�������루����˾��������,����˾��app���棩
{
    bool need_to_search = 1;
    string numx;
    cout<<"����������ĳ��ƺţ��ҽ�Ϊ���ṩ�˳�����-w-��";
    cin>>numx;
    if(car_map.count(numx)){
        PII tmp = car_map[numx];
        car_map.erase(numx);    //���
        int parking_NUMber = tmp.first,pile_Num = tmp.second;
        int in_space;
        for(int i=0;i<P[parking_NUMber].Get_concrete_vec()[pile_Num].size();i++){
            if(numx==P[parking_NUMber].Get_concrete_vec()[pile_Num][i].Get_car_number()){
                in_space = i;   ///������ͣ��λ��Ϊ���в�Ϊ�յ�����ṩ��Ϣ
                double buf_pay = take_charge_rate * (time(0) - P[parking_NUMber].Get_concrete_vec()[pile_Num][i].Get_Begin_Time());
                bool is_price = 0;
                if(Lucky_number[0]!='\0' && Lucky_number == P[parking_NUMber].Get_concrete_vec()[pile_Num][i].Get_SpaceName())
                {
                    cout<<"Oh���ó�λ�����˳�λ����Ϊ��������Ż�~~\n";
                    buf_pay /= 2.0;
                    is_price = 1;
                }
                Final_Income += buf_pay;
                printf("�ĸ�����ͣ����СƱ\n");
                printf("���ƺ���:%s\n", numx.c_str());
                printf("ͣ����:%c��ͣ����\n", 'A' + parking_NUMber);
                printf("��λ���:%s\n", P[parking_NUMber].Get_concrete_vec()[pile_Num][i].Get_SpaceName().c_str());
                printf("ͣ����ʼʱ�䣺%s\n",P[parking_NUMber].Get_concrete_vec()[pile_Num][i].Get_Begin_string().c_str());
                printf("ͣ������ʱ�䣺%s\n",printDate().c_str());
                printf("��ͣ��ʱ�䣺%d��\n",time(0) - P[parking_NUMber].Get_concrete_vec()[pile_Num][i].Get_Begin_Time());
                printf("�Ƿ��н���%d(1Ϊ�н�)\n", is_price);
                printf("�ϼ��տ%.2f\n", buf_pay);
                printf("лл���Ĺ��٣�ף���������\n");
                printf("����뿪��·����");
                find_shortest_way(parking_NUMber+1,1);
                P[parking_NUMber].Get_concrete_vec()[pile_Num][i].Space_clear();
                P[parking_NUMber].Get_space_res()++;
                res_space++;/*������bug*/
                break;
            }
        }
        Customer_Feedback();    //���˳���˾�����û�����
        if(!Q.empty()){
            need_to_search = 0;
            succeed_in(need_to_search,tmp,in_space);
        }
    }
    else{
        cout<<"���ĳ����������ĸ�����ͣ�������ȷ�Ϻ���Ϣ���ٽ��в���\n";
        Sleep(3000);
        return ;
    }
    Sleep(5000);
}

void Director::big_number(int num)    ///���еĴ�С����ջ����num��Ȼ������ַ������
{
    stack<int> stack_num;
    char c[20] = {0};//��ʼ��
    while(num){
        stack_num.push(num%10);
        num/=10;
    }
    int t = 0;
    while(!stack_num.empty()){
        c[t++] = stack_num.top() + '0';
        stack_num.pop();
    }
    struct number
    {
        char model[24][48];
    }number_M[10];
    memset(number_M,0,sizeof(number_M));
    int times = 2;
    int nums[20] = {0};
    for(int i=0;i<strlen(c);i++){
        nums[i] = c[i] - '0';
    }
    ///Ԥ���� �õ�times��nums����
    char buf_hor[24] = {0};
    buf_hor[0]= ' ';
    for(int i=1;i<=times;i++){
        buf_hor[i] = '-';
    }
    buf_hor[times+1] = ' ';
    ///����
    for(int i=0;i<=9;i++){
        //if(i==1)continue;
        for(int j=0;j<=2*times+2;j++){
            for(int k=0;k<=times+1;k++){
                number_M[i].model[j][k] = ' ';
            }
        }
    }
    ///�ո�
    for(int i=0;i<10;i++){
        if(i==2||i==3||i==5||i==6||i==7||i==8||i==9||i==0){}
        else continue;
        sprintf(number_M[i].model[0],buf_hor);
    }
    for(int i=0;i<10;i++){
        if(i==2||i==3||i==5||i==4||i==6||i==8||i==9){}
        else continue;
        sprintf(number_M[i].model[times+1],buf_hor);
    }
    for(int i=0;i<10;i++){
        if(i==2||i==3||i==5||i==0||i==6||i==8||i==9){}
        else continue;
        sprintf(number_M[i].model[2*times+2],buf_hor);
    }

    for(int i=0;i<10;i++){
        if(i==4||i==5||i==6||i==8||i==0||i==9){}
        else continue;
        for(int j=1;j<=times;j++){
            number_M[i].model[j][0] = '|';
        }
    }
    for(int i=0;i<10;i++){
        if(i==1||i==2||i==3||i==4||i==8||i==0||i==9||i==7){}
        else continue;
        for(int j=1;j<=times;j++){
            number_M[i].model[j][times+1] = '|';
        }
    }
    for(int i=0;i<10;i++){
        if(i==2||i==6||i==0||i==8){}
        else continue;
        for(int j=times+2;j<=2*times+1;j++){
            number_M[i].model[j][0] = '|';
        }
    }
    for(int i=0;i<10;i++){
        if(i==1||i==3||i==4||i==5||i==6||i==7||i==0||i==8||i==9){}
        else continue;
        for(int j=times+2;j<=2*times+1;j++){
            number_M[i].model[j][times+1] = '|';
        }
    }
    ///��ģ��
    for(int i=0;i<=2*times+2;i++){
        bool flag = 1;
        for(int j=0;j<strlen(c);j++){
            if(!flag) printf(" ");
            flag = 0;
            printf("%s",number_M[nums[j]].model[i]);
        }
        puts("");
    }
}

void Director::dijk_init()  //ͼ���㷨��ʼ��
{
    eid = 0;
    memset(p,-1,sizeof(p));
}

void Director::insert1(int u,int v,int w)///һ����uָ��v�������
{///��in_out����0��ʱ����Ǽ�����ڣ�1Ϊ����
        edge[eid].v = v;
        edge[eid].w = w;
        edge[eid].next = p[u];
        p[u] = eid++;
        flag_dijk = 1;  ///������
}

void Director::insert2(int u,int v,int w)///һ����u��v�������
{
    insert1(u,v,w);
    insert1(v,u,w);
}

void Director::delete_point(int s)    ///�ݻ�ͣ����s��ͬʱɾ��ͣ������Ӧ�ı� /*��δͶ��Ӧ����*/
{
    for(int i=0;i<MAX_N;i++){
        if(i==s){
            p[i] = -1;
        }
        else{
            int pre = i;
            for(int j=p[i];j!=-1;j=edge[j].next){
                if(edge[j].v==s){
                    if(j==p[i]){
                        p[i] = edge[j].next;
                    }
                    else{
                        edge[pre].next = edge[j].next;
                    }
                    break;
                }
                pre = j;
            }
        }
    }
    flag_dijk = 1;  ///ɾ����
}

bool Director::dijkstra_opt(int s,int n,bool in_out)    ///s������ڡ����������ͨͼ���򷵻�0����Ϊ�Ƿ���ͣ�������������ж�
//n�ĸ���Ӧ����ͣ��������+2�����ϳ���100�����0
{///in_out=0����ڵ�Դ���·��=1�ǳ��ڵ�Դ���·
    set<PII,less<PII> > min_heap;  ///�ü�����αʵ��һ��С���ѣ�ͬʱ��ӳ�����ѵĹ��ܡ�
    int vis[MAX_N];// ���ÿ�������Ƿ��ڼ��� U ��
    for(int i=0;i<MAX_N;i++){
        dist[i][in_out]=INF;
        new_road[i][in_out] = -1; ///Ϊ�����������׼��
        vis[i]=0;
    }
    min_heap.clear();
    ///���ݳ�ʼ��
    min_heap.insert(make_pair(0,s));
    dist[s][in_out] = 0;    ///��ʼ����Ϊ0
    for(int i=0;i<n;i++){
        if(min_heap.size()==0){// ���С������û�п��ö��㣬˵���ж����޷���Դ�㵽��㷨����
            return false;
        }
        set<PII,less<PII> >::iterator it = min_heap.begin();
        int u = it->second;
        min_heap.erase(*it);
        vis[u] = true;
        // ���к���ͨ dijkstra �㷨���Ƶ��ɳڲ���
        for(int j=p[u];j!=-1;j=edge[j].next){
            int v = edge[j].v;
            if(!vis[v]&&dist[v][in_out]>dist[u][in_out]+edge[j].w){
                min_heap.erase(make_pair(dist[v][in_out],v));
                dist[v][in_out] = dist[u][in_out] + edge[j].w;
                new_road[v][in_out] = u;///��¼·��
                min_heap.insert(make_pair(dist[v][in_out],v));
            }
        }
    }
    return true;
}

void Director::find_shortest_way(int s,bool in_out)   /*���·�����������λ�ûд��*/
{
///�����ͣ������·�����Խ�����޸�һ�¡�
    if(in_out==0){
        int t = 0;
        int way[100] = {0};
        way[t++] = s;
        while(new_road[s][0]!=-1){
            way[t++] = new_road[s][0];
            s = new_road[s][0];
        }
        way[t] = ENTRANCE;
        printf("ROADS:����");
        for(int i=t-2;i>=0;i--){
            printf("--%d��ͣ����",way[i]);
        }
        printf("\n");
    }
    else{
        printf("ROADS:");
        while(new_road[s][1]!=-1){
            printf("%d��ͣ����--",s);
            s = new_road[s][1];
        }
        printf("����\n");
    }
}

void Director::MAP_init()   //��ͼ��ʼ��
{
    memset(MAP,0,sizeof(MAP));
    memset(legal_map,0,sizeof(legal_map));
    for(int i=0;i<L;i++) for(int j=0;j<R;j++) MAPX[i][j] = '.'; //�հ�����
}

void Director::Parking_draw() //ͣ�������ڵ�ͼ��
{
    int n = P.size();//ͣ��������

    for(int i=0;i<9;i++) MAP[L-1][i] = MAP[L-2][i] = MAP[L-3][i] = -100,legal_map[L-1][i] = legal_map[L-2][i] = legal_map[L-3][i] = 1,MAPX[L-1][i] = MAPX[L-2][i] = MAPX[L-3][i] = '^';  //���
    for(int i=R-1;i>R-10;i--) MAP[0][i] = MAP[1][i] = MAP[2][i] = -200,legal_map[0][i] = legal_map[1][i] = legal_map[2][i] = 1,MAPX[0][i] = MAPX[1][i] = MAPX[2][i] = '^';  //����

    for(int k=0;k<n;k++){
        site buf = P[k].Return_Location();
        int b_l = buf.x-buf.l,b_ll = buf.x+buf.l,b_r = buf.y-buf.r,b_rr = buf.y+buf.r;
        for(int i=b_l-1;i<=b_ll+1;i++){
            for(int j=b_r-1;j<=b_rr+1;j++){
                if(in(i,j)){
                    if(b_l<=i&&i<=b_ll&&b_r<=j&&j<=b_rr){
                        MAP[i][j] = k + 1;
                        MAPX[i][j] = '0' + k + 1;
                    }
                    else{
                        if(is_road(Point(i,j,0))) {}    //����ǵ�·������������ػ�·��
                        else MAP[i][j] = ILLEGAL_NUM,MAPX[i][j] = '!';    //�Ƿ�����
                    }
                    legal_map[i][j] = 1;    //����1����ռ�ã����Ƿ�
                }
            }
        }
        MAP[buf.x-buf.l+2][buf.y-buf.r] = MAP[buf.x+buf.l-2][buf.y-buf.r] = -k-1;
        MAP[buf.x-buf.l+2][buf.y+buf.r] = MAP[buf.x+buf.l-2][buf.y+buf.r] = -k-1;
        MAP[buf.x-buf.l][buf.y+buf.r-2] = MAP[buf.x-buf.l][buf.y-buf.r+2] = -k-1;
        MAP[buf.x+buf.l][buf.y+buf.r-2] = MAP[buf.x+buf.l][buf.y-buf.r+2] = -k-1;

        MAPX[buf.x-buf.l+2][buf.y-buf.r] = MAPX[buf.x+buf.l-2][buf.y-buf.r] = '#';
        MAPX[buf.x-buf.l+2][buf.y+buf.r] = MAPX[buf.x+buf.l-2][buf.y+buf.r] = '#';
        MAPX[buf.x-buf.l][buf.y+buf.r-2] = MAPX[buf.x-buf.l][buf.y-buf.r+2] = '#';
        MAPX[buf.x+buf.l][buf.y+buf.r-2] = MAPX[buf.x+buf.l][buf.y-buf.r+2] = '#';
    }
}

void Director::MAP_legal_show() //��ѡ��ͼչʾ
{
    for(int i=0;i<L;i++){
        for(int j=0;j<R;j++){
            printf("%c",MAPX[i][j]);
        }
        printf("\n");
    }
    printf("\nע�ͣ�.Ϊ�հ�����#Ϊͣ��������,!Ϊ�Ƿ�����i(1~9)Ϊi��ͣ������ռλ��;\n");
    printf("^�ǳ���ڣ�*�ǵ�·\n");
}

void Director::MAP_show()   //��ͼչʾ
{
    if(!is_project){
        cout<<"����Ƥ��û�У������ĵ�ͼ��������\n";
        return;
    }
    for(int i=0;i<L;i++){
        for(int j=0;j<R;j++){
            printf("%c",MAPX[i][j]);
        }
        printf("\n");
    }
    printf("\nע�ͣ�.Ϊ�հ�����#Ϊͣ��������,!Ϊ�Ƿ�����i(1~9)Ϊi��ͣ������ռλ��;\n");
    printf("^�ǳ���ڣ�*�ǵ�·\n");
    printf("�Ƿ�Ҫ�˳��鿴�����س����˳�(����ȴ�3��):");
    getchar();
    getchar();
}

void Director::Parking_delete(){}  /*ɾ����·��dfs*/

bool Director::is_road(const Point &A)    //�ж��Ƿ�Ϊ·
{
    return MAP[A.x][A.y]>ILLEGAL_NUM&&MAP[A.x][A.y]<=20001;
}

void Director::find_start_points(const site &A,const site &B,Point &a1)   //A->B��������ͣ���������λ�÷ֳ�����������жϷ��򣬱����Զ����ɵ�·�ƽ���һȦ
{
    int d_x = A.x - B.x , d_y = A.y - B.y;
    bool nor=0,wes=0,eas=0,sou=0;
    if(abs(d_y)>abs(d_x)){
        if(d_y<0) eas = 1;
        else wes = 1;
    }
    else{
        if(d_x<0) sou = 1;
        else nor = 1;
    }
    if(sou){
        int i = A.x+A.l+1;
        for(int j=A.y-A.r+1;j<=A.y+A.r-1;j++){
            if(in(i,j)&&!is_road(Point(i,j))){
                a1 = Point(i,j,0);
                return ;
            }
        }
    }

    if(nor){
        int i = A.x-A.l-1;
        for(int j=A.y-A.r+1;j<=A.y+A.r-1;j++){
            if(in(i,j)&&!is_road(Point(i,j))){
                a1 = Point(i,j,0);
                return ;
            }
        }
    }

    if(wes){
        int j = A.y-A.r-1;
        for(int i=A.x-A.l+1;i<=A.x+A.l-1;i++){
            if(in(i,j)&&!is_road(Point(i,j))){
                a1 = Point(i,j,0);
                return ;
            }
        }
    }

    if(eas){
        int j = A.y+A.r+1;
        for(int i=A.x-A.l+1;i<=A.x+A.l-1;i++){
            if(in(i,j)&&!is_road(Point(i,j))){
                a1 = Point(i,j,0);
                return ;
            }
        }
    }

}

bool Director::legal_for_road(const Point&A)  //������·�ĵط�������vis_bfs����ĳ�ʼ��
{
    return (MAP[A.x][A.y]>=ILLEGAL_NUM&&MAP[A.x][A.y]<=20001)||MAP[A.x][A.y]==0;
}

void Director::dye_track(int u,int v,Point &finalx)    //·��Ⱦɫ��ʹ�ù�ϣӳ�䣬ͬʱ��Ⱦɫֵ����set�����С�
{
    int color_num = (u*10086+v*10010)%10000+10001;
    colx.insert(color_num);
    OP sta = op[finalx.ind];

    while(sta.pre!=-1){  //��������ʽǰ���ǻ�û��ͷ
        MAP[sta.x][sta.y] = color_num;
        MAPX[sta.x][sta.y] = '*';
        sta = op[sta.pre];
    }
    MAP[op[0].x][op[0].y] = color_num;
    MAPX[op[0].x][op[0].y] = '*';
    return ;
}

void Director::vis_init()
{
    for(int i=0;i<L;i++){
        for(int j=0;j<R;j++){
            if(legal_for_road(Point(i,j))){
                vis_bfs_dfs[i][j] = 0;
            }
            else{
                vis_bfs_dfs[i][j] = 1;
            }
        }
    }   //��ʼ��
}

int Director::bfs_opt(site A,site B)  //bfs��·�㷨
{
    vis_init();
    if(B.number==0) B.number = -100;
    if(B.number==100) B.number = -200;//�ı��������
    memset(op,0,sizeof(op));
    for(int i=0;i<L;i++){
        for(int j=0;j<R;j++){
            if(legal_for_road(Point(i,j))){
                vis_bfs_dfs[i][j] = 0;
            }
            else{
                vis_bfs_dfs[i][j] = 1;
            }
        }
    }//vis��ʼ��
    queue<Point> Q_bfs;
    while(!Q_bfs.empty()) Q_bfs.pop();
    Point start_p;
    int id=0;       ///Ϊ��ʽǰ������׼��
    op[id++] = OP(start_p.x,start_p.y,-1);  //����ĵ�
    find_start_points(A,B,start_p);
    start_p.ind = 0;
    Q_bfs.push(start_p);
    //cout<<"test:"<<start_p.x<<" "<<start_p.y<<" "<<start_p.ind<<"\n";
    Point finalx;
    int cur = 0,last = 1,step = 0;  ///����·�����ȼ�¼���ܺ����Ͳ������ˡ�cur���ִ�ĵ㣬last��ʣ��ĵ㣬step������
    while(!Q_bfs.empty()){
           // puts("TEST1-----------------");
        Point st = Q_bfs.front();
        int ID = st.ind;
        Q_bfs.pop();
        last--;
        int countx = 0;
        for(int i=rand()%4;countx<4;i++,i%=4,countx++){
            int tx = st.x + dir[i][0],ty = st.y + dir[i][1];
            if(!in(tx,ty)) continue;    //Խ������Ҫ����
            if(MAP[tx][ty]==B.number){
                finalx = Point(st.x,st.y,st.ind);
               // puts("test2:::::");
                dye_track(A.number,B.number,finalx);
                return step;
            }
            if(!vis_bfs_dfs[tx][ty]){
                vis_bfs_dfs[tx][ty] = 1;
                cur++;
                op[id] = OP(tx,ty,ID);
                Q_bfs.push(Point(tx,ty,id++));
            }
        }
        if(last==0){
            last = cur;
            cur = 0;
            step++;
        }
    }
    puts("����TEST2_bfs-----------------����Ӧ��ʾ�Ĵ���");
}

void Director::lottery_system()   //�ý�ά�������Ż��㷨
{
    if(!P.size()){
        cout<<"��ǰû�г�λ�������������˳�λ�������ع����߽��棩"<<"\n";
        return;
    }

    int ramx = rand()%total_space;   ///�����г�λ
    int i=0,j=0,k=0;
    while(ramx>P[i].Get_num()){  //�ȿ����ҵ�����Ӧ�ĳ�λ�����������
        ramx-=P[i].Get_num();
        i++;
    }
    while(ramx>P[i].Get_concrete_vec()[j].size()){  //�ȿ����ҵ�����Ӧ�ĳ�λ�����������
        ramx-=P[i].Get_concrete_vec()[j].size();
        j++;
    }
    while(ramx>1){  //�߽紦��
        ramx--;
        k++;
    }
    Lucky_number = P[i].space_NName(j,k);
    printf("���˳�λ�ĺ����ǣ�%s\n",Lucky_number.c_str());
    printf("��˾����������˳�λ��ʱ�򣬽���ʱ�ɴ�����\n");
    return ;
}

void Director::prime_game_system()  //��Ϸϵͳ
{
	int begin = 0, end = 0, times = 1, clock = 0;
	int number, judge = -1, set = -1;
	int n = 0;
	int guess = -1;
	srand((int)time(NULL));
	number = rand() % 1000;
	cout << "��Ϸ���ƣ���������Ϸ\n��Ϸ����ϵͳ����0��1000���������һ����������Ҫ������������Ƕ��٣�ÿ�����������ֺ����ǻ���ʾ���������ƫ����ƫС��";
	cout << "��ֻҪ��30�����ڻ���15�β���֮�ڲ�����ϵͳ���ɵ�����������㽫���ʤ��" << endl;
	cout << "�������������ʹ�ù��ܣ�\n1.��ʼ��Ϸ\n2.�ص��û�����\n3.ϵͳ����" << endl;
	cout << "������ʹ������ܣ�";
	cin >> judge;
	L1:while (1) {
		if (judge == 1)
			break;
		if (judge == 2)
			show_customer_content();
			system("cls");
			cout<<"���ص��û�����...\n";
			Sleep(3000);
			return ;
		if (judge == 3) {
			system("cls");
			cout << "���������ʹ�����ã�\n1.�޸��������Χ\n2.�˳�����" << endl;
			cout << "������ʹ���������ã�";
			cin >> set;
			judge = -1;
			while (1) {
				if (set == 1) {
					cout << "������������ķ�ΧΪ��0��N\n������N:";
					cin >> n;
					number = rand() % n;
					system("cls");
					break;
				}
				if (set == 2) {
					system("cls");
					break;
				}
			}
			goto L1;
		}
		cout << "�������������ʹ�ù��ܣ�\n1.��ʼ��Ϸ\n2.�˳�ϵͳ\n3.ϵͳ����" << endl;
		cout << "������ʹ������ܣ�";
		cin >> judge;
		system("cls");
	}
	cout << "������Ѿ����ɣ���Ϸ�����������һ�����ֺ��ʱ\n������һ�����֣�" << endl;
	cin >> guess;
	begin = (int)time(NULL);
	while (guess != number) {
		times++;
		system("cls");
		if (guess > number) {
			cout << "��������������" << guess << endl;
			cout << "���µ�����̫���ˣ����������ٲ�һ�����֣�" << endl;
		}
		else {
			cout << "��������������" << guess << endl;
			cout << "���µ�����̫С�ˣ����������ٲ�һ�����֣�" << endl;
		}
		cin >> guess;
	}
	end = (int)time(0);
	clock = end - begin;
	cout << "�ڱ�����Ϸ��\n���ܹ�����ʱ�䣺" << clock << "��" << endl;
	cout << "���ܹ����Բ����Ĵ�����" << times << "��" << endl;
	if (clock <= 30 || times <= 15)
		cout << "Congratulations,you win!!!" << endl;
	else
		cout << "Sorry,you lose." << endl;
}

void Director::do_business()    //��ʼӪҵ
{
    if(!P.size()){
        cout<<"��ͣ������û�У����ܿ�ʼӪҵ��\n";
    }
    else{
        cout<<"��ʼӪҵ\n";
        is_running = 1; //��ʼӪҵ
    }

}

void Director::Customer_Feedback()    //�û�����
{
    printf("�ٴθ�л�����������ĸ�����ͣ�����ķ�������������д�������Ƿ�������������1or0\n");
    bool buf;
    cin>>buf;
    string s1;
    if(buf){
        printf("�����ÿ��Ը������ĸ�����ͣ���������ٷ��أ���0~100��:");
        int mark;
        scanf("%d",&mark);
        printf("Ը���������������1or0:");
        int buf2;
        scanf("%d",&buf2);
        if(buf2){
            cout<<"��������ǣ�";
            getchar();
            getline(cin,s1);
        }
        else{
            s1 = "\0";
        }
        Rank.get_in_feedback(mark,s1);
    }
    printf("��л���ĵ�������ӭ�´ι���\n");
    //show_main_content();����Ҫд
}

void Director::show_Ranked_list()
{
    if(Rank.Get_num()){
        Rank.Rank_show();
    }
    else{
        printf("Ŀǰ��û�г����������������ۣ�\n");
    }
}

void Director::Space_search_and_insert(string &space_NAME,int &search_num)//�ṩ���һ�·������Ϣ
{
    int minn = INF; //����С�����ʼΪ����ֵ��Ȼ��ö������ͣ�������ҵ���������Ŀ��Բ�����ͣ����
    for(int i=0;i<P.size();i++){
        if(P[i].Get_space_res()>0){
            if(minn>dist[i+1][0])   ///dist[i+1][0]�����i+1��ͣ���������0����̾���
            {
                minn = dist[i+1][0];
                search_num = i;//һ���н⣬���������еĳ���ԭ����Ȼ����һ��ͣ������ʣ�೵λ����0
            }
        }
    }/*������*/
    for(int i=0;i<P[search_num].Get_concrete_vec().size();i++){
        for(int j=0;j<P[search_num].Get_concrete_vec()[i].size();j++){
            if(P[search_num].Get_concrete_vec()[i][j].Get_State()==0){
                P[search_num].Parking_car(i,j,1,Q.front().number,time(0));/*֮��һЩ��Ϣ����λ��*/
                P[search_num].Get_concrete_vec()[i][j].Get_Begin_string() = printDate();//����Ʊ�ṩ��Ϣ
                space_NAME = P[search_num].space_NName(i,j);
                P[search_num].Red_space_res();//ͣ������λ����
                res_space--;//�ܳ�λҲҪ����
                car_map[Q.front().number] = make_pair(search_num,i);
                return ;
            }
        }
    }
}

string Director::printDate()
{
    char buff1[256];
    char buff2[256];
    char buff3[256];
    char buff4[256];
    char buff5[256];
    char buff6[256];
    time_t t = time(0);
    strftime(buff1, sizeof(buff1), "%Y", localtime(&t));
    strftime(buff2, sizeof(buff2), "%m", localtime(&t));
    strftime(buff3, sizeof(buff3), "%d", localtime(&t));
    strftime(buff4, sizeof(buff4), "%H", localtime(&t));
    strftime(buff5, sizeof(buff5), "%M", localtime(&t));
    strftime(buff6, sizeof(buff6), "%S", localtime(&t));
    int year=atoi(buff1),month=atoi(buff2),day=atoi(buff3);
    int hour=atoi(buff4),minute=atoi(buff5),second=atoi(buff6);
    char s1[100];
    sprintf(s1,"%d��%d��%d��%dʱ%d��%d��\n",year,month,day,hour,minute,second);
    string s2 = s1;
    return s2;
}

void Director::Class_Test()
{
    cout<<"Ŀǰû����Ҫ���Եĵط�\n";
}

void Director::change_charge()
{
    cout<<"���뽫�շ�ϵ���ĳɶ����أ��������Ƽ�0.05~0.5��\n";
    cin>>take_charge_rate;
    cout<<"�޸����\n";
}

void Director::show_money()
{
    cout<<"ĿǰӪ��Ϊ��"<<Final_Income<<'\n';
}

void Director::ADD_TRACK()
{
    cout<<"(�Ƿ�Ҫ�鿴��ͼ(0Ϊ���Խ��죬1Ϊ����)����y=1,n=0��)��";
    bool flagx;
    cin>>flagx;
    if(flagx){
        MAP_legal_show();
    }
    cout<<"��������ͨ�������㣨�����0��������100����u��v����";
    int u,v;
    while(cin>>u>>v&&(u==v||(u>P.size()&&u!=100)||(v>P.size()&&v!=100))){
        printf("u��v������ͬ��Ҫ�Ϸ����������룺");
    }

    site buf1,buf2;
    if(u==0) buf1 = B;
    else if(u==100) buf1 = E;
    else buf1 = P[u-1].Return_Location();
    if(v==0) buf2 = B;
    else if(v==100) buf2 = E;
    else buf2 = P[v-1].Return_Location();
    //printf("test6:%d %d,%d %d\n",buf1.x,buf1.y,buf2.x,buf2.y);
    int w = bfs_opt(buf1,buf2); ///�������·���ĳ��ȣ�������ͼ�еı�Ȩ��
    insert2(u,v,w);
    dijkstra_opt(0,P.size()+2,0);
    dijkstra_opt(100,P.size()+2,1);
    cout<<"�ɹ���ͨ���أ�\n";
    Sleep(1500);
    system("cls");
}

#ifdef finish
int main(int argc,char*argv[])
{
    srand(time(0));
    Director execute;
    execute.show_main_content();
    return 0;
}
#endif
