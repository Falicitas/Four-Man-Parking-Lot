#include<bits/stdc++.h>
#include<windows.h>
using namespace std;

/*******************测试*********************/
#define finish 1
//#define Test
/************全局常量以及命名***************/
typedef pair<int,int> PII;  //映射对

const int MAX_N = 105;//最多9个停车场个数，0是入口，100是出口。（防止越界）
const int ENTRANCE = 0,EXIT = 100;  //入口，出口的编号
const int MAX_M = 10000;//最多的停车场的边数
const int MAX_MM = 205; //地图的边界小于等于200
const int INF = 0x3f3f3f3f;
const double eps = 1e-9;
const int ILLEGAL_NUM = 10000;  //当二维地图的坐标值大于10000，或者=0，都是合法建造区域

const int dir[4][2] = {-1,0,1,0,0,1,0,-1};  //四个方向，给bfs，dfs二维搜索提供固定方向

/**************各种类***************/

class Ranked_list
{
private:
    int num = 0;    //评价的总数
    int Excellent = 0;
    int Fine = 0;
    int Ordinary = 0;
    int Need_improvement = 0;
    int Disappointed = 0;
    int Total_sum = 0;
    double Average_mark = 0;
    vector<string> Feedback;    //用户的意见
public:
    int Get_num()
    {
        return num; //如果num==0，管理者不能查看
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
            cout<<"用户的评价有：\n";
            for(int i=0;i<Feedback.size();i++){
                cout<<Feedback[i]<<"\n";
            }
        }
        printf("----------------------------------------\n");
        printf("****************************************\n");
        Sleep(2000);
    }
};

struct EDGE    //边的信息，用链式数组储存
{
    int v,w;  //e为点，w为边的权值
    string road;    ///街道的名字，为司机提供信息
    int next;
};

struct car  //车辆信息（内含车牌号）
{
	string number;
	//string car_brand;
	car(string s1)
	{
	    number = s1;
	}
};

struct site //停车场的大小以及中心坐标（地址）
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
	int Space_Number;//车位编号
	string Space_Name;//与space_number相关联
	string car_number;//停车号码
	string Begin_string;    //储存进入的时间信息
	bool b=0;//是否占用
	long long Begin_Time;//进入时间
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
	    return Space_Number;//int型
    }//得到车位号，给中奖车位做准备
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
	int space_num=0;  //停车场车位总个数
	int space_res;  //剩余车位个数
public:
	Parking_Lot(const Parking_Lot & A) {
        s = A.s;
        for(int i=0;i<A.vec.size();i++) vec.push_back(A.vec[i]);
        space_num = A.space_num;
        space_res = A.space_res;
    }
	Parking_Lot(const site &s,const vector<int> pile_num):s(s)
	{   //为director里的vector推入做准备，同时推入车位空间，给车位命名
	    for(int i=0;i<pile_num.size();i++){
            vector<Space> buffff;
            for(int j=0;j<pile_num[i];j++){
                string space_name(10,'\0');//停车位名字
                space_name[0] = 'A' + s.number - 1;
                space_name[1] = '-',space_name[2] = i + '0' + 1,space_name[3] = '-';
                int buf = j,ind = 6;
                while(buf){
                    space_name[ind--] = buf % 10 + '0';
                    buf/=10;
                }
                while(ind!=3){
                    space_name[ind--] = '0';
                }   ///最后能得到一个字符串，例:A-2-010车位
                ///意思是A号停车场，第二层，第10号车位
                buffff.push_back(Space(0,space_num++,space_name));
                ///当增加一个车位，停车场车位个数增加
            }
            vec.push_back(buffff);
	    }
	    space_res = space_num;  ///初始停车场剩余的个数等于总车位数
	}
	site &Return_Location() {//返回停车场地址（坐标和长度宽度）
        return s;
    }
    int &Get_num()
    {
        return space_num;
    }
    int &Get_space_res() //返回剩余车位数量
    {
        return space_res;
    }
    vector<vector<Space> > &Get_concrete_vec()
    {
        return vec;
    }
    string &space_NName(int PILX,int indx)   ///返回车位的名称
    {
        return vec[PILX][indx].Get_SpaceName();
    }
    void Red_space_res()    ///减少剩余车位数量
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

struct OP///用反向链式前向星来储存最短回路的路径信息
{
    int pre=0;
    int x,y;
    OP(int x=0,int y=0,int pre=0):x(x),y(y),pre(pre){}
};

/************指挥者***************/
class Director
{
private:
    ///大型游乐场的停车场系统基本数据
    int L,R;    //该项目所占的地皮场和宽
    bool is_project = 0;    //当确认了大型停车场的L与R，才能算是立项了
    bool is_running = 0;
    double take_charge_rate = 0.2;//默认收费系数
    string cipher = "666dlut";
	vector<Parking_Lot> P;//最多十个停车场,下标+1=编号
	Ranked_list Rank;//评分类对象
	double Final_Income = 0;//停车场收入
	int total_space = 0;//停车场车位总数 //维护的时候写吧
	int res_space = 0;    //停车场车位剩余总数
	queue<car> Q;   //车队
	map<string,PII> car_map;//储存车辆在停车场的信息。用映射二叉堆来表示“车号”、“几号停车场”、“第几层”。优化了车主出停车场的函数搜索算法
	string Lucky_number = "\0";  //中奖（车位）制度服务
	set<int> colx;//路径颜色标记
	///图论数据
	site B,E;   //入口与出口的地址
	EDGE edge[MAX_M]; //有向边的信息
	bool flag_dijk = 0; //判断是否需要调用dijkstra
	int eid,p[MAX_N];//链式前向星的头指针数组
    int dist[MAX_N][2];// 存储单源最短路的结果
    int new_road[MAX_N][2];//用来记录每个点最后由谁来更新的，从此找出从入口(出口)到停车位的最短路径
    ///二维地图数据
    bool vis_bfs_dfs[MAX_MM][MAX_MM];
    int MAP[MAX_MM][MAX_MM];//大型停车场二维地图制作
    char MAPX[MAX_MM][MAX_MM];
    bool legal_map[MAX_MM][MAX_MM];
    OP op[MAX_MM*MAX_MM];   //bfs路径信息

    //mainmenu w;

public:
    /************函数目录索引（抽象建造者）***************/
    ///界面显示
    void show_main_content();   //主界面
    void show_manager_content();    //管理者界面
    void show_customer_content();   //顾客界面
    ///停车场车辆与车位管理
    void L_R_setting(); //项目的立项
    void Space_search_and_insert(string &space_NAME,int &search_num);//提供给找回路径的信息
    bool Legal_Parking_building();//是否能成功建成一个停车场
    bool in(int x,int y);//判断是否越界访问
    string printDate();//提供当前时间的字符串数据
    void add_Space();   /*尚未投入应用中,且未实现*/
    ///面向用户
    void check_in();
    void succeed_in(bool need_to_search,const PII &in_pile,const int &in_space);//停车场已经建好了，不需要调用dijk（同时这不是司机操作的）;//不是用户操作的
    void check_out();
	///图论算法
    void dijk_init();//使用之前的初始化
    void insert1(int u,int v,int w);//一条由u指向v的有向边///当in_out等于0的时候就是计算入口，1为出口
    void insert2(int u,int v,int w);//一条由u指向v的无向边///当in_out等于0的时候就是计算入口，1为出口
    void delete_point(int s);    /*(其不仅是图论，还是数据结构，还涉及二维地图的处理)*/ /*尚未投入应用中*/
    bool dijkstra_opt(int s,int n,bool in_out);    //s代表入口。如果不是连通图，则返回0，作为非法的停车场管理做出判断
    void find_shortest_way(int s,bool in_out);   //最短路径返回
    bool is_road(const Point &A);   //判断是否是道路，以免重画
    ///二维地图制作以及bfs最短路径制作算法
    void MAP_init();//地图初始化
    void Parking_draw();
    void MAP_show();//地图展示，包括停车场以及道路、景观的展示
    void MAP_legal_show();//合法地图展示，观察哪些区域可以建设
    void Parking_delete();  /*删除道路用dfs*/
    int bfs_opt(site A,site B);
    ///中奖与游戏系统
    void lottery_system();
    void prime_game_system();
    ///维护系统（此时停车场禁止营业）
    void safeguard();//停车场维护
    void do_business();//维修模式的时候要把停车场的车主给请出大型停车场
    ///杂项函数
    void big_number(int num);   //将发票用大数字方式显示，显得明显
    void show_Ranked_list();    //展示停车场的用户评价
    void Customer_Feedback();   //用户的反馈
    void show_info();   //展示给管理者基本信息
    bool TEST(const site &test);    //停车场合理性测试
    void find_start_points(const site &A,const site &B,Point &a1);   //找到开始的方向。
    bool legal_for_road(const Point&A);
    void dye_track(int u,int v,Point &finalx);
    void vis_init();
    void Class_Test();  //测试用
    void change_charge();   //修改收费系数
    void show_money();  //营业额输出
    void interface_show();
    void ADD_TRACK();
};

void Director::show_main_content()
{
    cout<<"----------------------------------------\n";
    cout<<"****************************************\n";
    printf("%-7s","四个猛男停车场APP界面\n");
    cout<<"****************************************\n";
    cout<<"----------------------------------------\n";
    cout<<"\n\n";
    cout<<"请问你是管理者还是车主呢？,车主选1，管理者选0(退出程序请按2):";
    int buf_nu;
    cin>>buf_nu;
    if(buf_nu==2){
        exit(0);
    }
    else if(buf_nu==1){ ///车主
        system("cls");
        show_customer_content();
    }
    else{
        system("cls");
        cout<<"请输入管理者的密码：";//密码正确才能进入管理界面
        string str;
        int kase = 3;
        while(cin>>str&&str!=cipher&&kase--){
            if(kase>0) cout<<"输入密码错误，请重新输入(还剩"<<kase<<"次)\n";
        }
        if(kase==-1){
            cout<<"错误次数过多，将返回主界面\n";
            Sleep(3000);
            system("cls");
            show_main_content();
            return ;
        }
        cout<<"密码输入正确，将进入管理者界面\n";
        Sleep(3000);
        system("cls");
        show_manager_content();
    }
}

void Director::show_manager_content()//面向管理者，需简洁，同时要足够自动化
{

    cout<<"您可操作的页面：\n";
    cout<<"A-1:项目的立项（若此停车场项目已立项，无需操作此项）;\n";
    cout<<"A-2:停车场的建立;\n";
    /*
    **同时建立了停车场后，默认会进入停车场维修模式，此时不对外营业
    */
    cout<<"A-3:四个猛男停车场的信息;\n";//返回给管理者目前有多少个停车场，每个停车场的信息是什么
    cout<<"A-4:开始对外营业;\n";
    /*至少要有一个停车场才能营业
    **同时包括入口与出口在内，该图得是个联通图才能营业（与维护挂钩）
    */
    cout<<"A-5:查看四个猛男停车场的客户评价;\n";//show_Ranked_list();
    cout<<"A-6:查看停车场的二维地图;\n";//show
    cout<<"A-7:随机抽选幸运车位;\n";
    cout<<"A-8:当前营业额显示;\n";
    cout<<"A-9:收费系数修改;\n";
    cout<<"A-10:返回主界面;\n";
    cout<<"A-11:测试板块;\n";
    cout<<"A-12:增加四个猛男停车场的边;\n";
    cout<<"-----------------------------------------------------\n";
    cout<<"请输入你想操作的序号(X)：";
    int s1;
    int kase = 3;
    while(cin>>s1&&!(s1>=1&s1<=12)&&kase--){  //添加功能的时候记得这里也要改
        if(kase>=0) cout<<"你输入的序号有误，你还有"<<kase+1<<"的输入机会:";
    }
    if(kase==-1){
        cout<<"错误次数过多，返回主界面\n";
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
            safeguard();    //如果停车场建设成功，则进入维护状态
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
        cout<<"将返回主界面...\n";
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
        cout<<"未知错误，退出程序\n";
        exit(1);
    }
    Sleep(3000);
    system("cls");
    show_manager_content();
}

void Director::show_customer_content()//面向客户，需尊重客户的需求
{
    cout<<"欢迎使用四个猛男停车场~我是你的向导\n";
    cout<<"你想要办理什么业务呢？\n";
    cout<<"B-1:办理入场手续;\n";
    cout<<"B-2:办理退场手续;\n";  //同时给客户提供反馈问卷
    cout<<"B-3:试玩四个猛男开发的小游戏（有机会获得优惠券哦）;\n";
    cout<<"B-4:返回主界面;\n";
    cout<<"-----------------------------------------------------\n";
    cout<<"请输入你想操作的序号(X)：";
    char s1;
    int kase = 3;
    while(cin>>s1&&!(s1>='1'&&s1<='4')&&kase--){
            if(kase>=0) cout<<"你输入的序号有误，你还有"<<kase+1<<"的输入机会:";
    }
    if(kase==-1){
        cout<<"错误次数过多，返回主界面\n";
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
        cout<<"将返回主界面...\n";
        Sleep(3000);
        system("cls");
        show_main_content();
        return;
    default:
        cout<<"未知错误，将退出程序\n";
        exit(1);
    }
        Sleep(3000);
        system("cls");
    show_customer_content();
}

void Director::L_R_setting()    //立项
{
    if(is_project==0){
        cout<<"出手阔绰的老板终于要立项了！请告诉你的伟大宏图：但我们建筑工人的水平有限，只能给你建造长宽≥120，≤155的停车场哦~（输入长与宽）：";
        cin>>L>>R;
        if(120<=L&&L<=155&&120<=R&&R<=155){
            is_project = 1;
            MAP_init(); //地图初始化
            dijk_init();    //图论算法初始化
            B = site(0,L-2,4,1,4);
            E = site(100,1,R-5,1,4);
            cout<<"成功立项！（返回管理者界面）\n";
        }
        else{
            cout<<"都说了长宽≥150，≤200了，你这是在为难我们建筑工人！（立项失败，返回管理者界面）\n";
        }
    }
    else{
        cout<<"你已经有一个项目了！做老板不要太贪心！(返回管理者界面)\n";
    }
    Sleep(3000);
    system("cls");
    show_manager_content();
}

bool Director::in(int x,int y)//判断是否越界访问
{
    return 0<=x&&x<L&&0<=y&&y<R;
}

bool Director::TEST(const site &test)//测试在二维地图上能否可以建停车场
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
    /*  只检测四点是有风险的，倘若中心区域有停车场，那么就是非法建设了
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

bool Director::Legal_Parking_building()  //建设停车场的合法与否，合法则建设停车场
{
    if(!is_project){
        cout<<"连地皮都没有，怎么建造停车场？？把这位穷老板铲出去！(返回管理者界面)\n";
        return false;
    }
    if(P.size()>=9){
        cout<<"停车场数目太多啦，目前的地皮不允许建造这么多停车场！(返回管理者界面)\n";
        return false;
    }

	cout<<"温馨提示：最好先查看一下地图，看看哪些区域适合新建停车场哦~\n";
    cout<<"(是否要查看地图？（yes=1,no=0）)：";
    bool flagx;
    cin>>flagx;
    if(flagx){
        MAP_legal_show();
    }
    cout<<"开始进入停车场建设流程...:\n";
	site test;
	cout<<"其中L是长半径，R是宽半径,即中心坐标向左向右各延伸R,向上向下各延伸L。建立L，R不超过6，不低于3\n";
	cout<<"输入坐标、L与R（x y L R）：";
	cin>>test.x>>test.y>>test.l>>test.r;
	test.number = P.size() + 1;
	if(TEST(test)){
        int pile;   //层数
        vector<int> pile_num;    //每一层车位个数
        cout<<"恭喜，该区域允许建造停车场！\n";
        cout<<"请告诉我你需要多少层（目前的建造水平只允许你最多构建三层）：";
        int kase = 0;//防捣乱
        while(scanf("%d",&pile)==1&&(pile<=0||pile>3)&&kase<=3){
            cout<<"用你的手指数：1,2,3！重输一次！:";
            kase++;
        }
        if(kase==4){
            cout<<"看来你是不想配合我们建筑工人的工作。来人，把他铲走！（停车场建造失败）\n";
            Sleep(3000);
            system("cls");
            return false;
        }
        for(int i=0;i<pile;i++){
            cout<<"请告诉我第"<<i+1<<"层所提供的车位(每层最少3个车位，最多20个车位！):";
            int kase = 0;
            int buffff;
            while(scanf("%d",&buffff)==1&&(buffff<3||buffff>20)&&kase<=3){
                cout<<"老板，你数学是不是不行？？请输入3~20的数字!:";
                kase++;
            }
            if(kase==4){
                cout<<"看来你是不想配合我们建筑工人的工作。来人，把他铲走！（停车场建造失败）\n";
                Sleep(3000);
                system("cls");
                return false;
            }
            pile_num.push_back(buffff);
        }
        P.push_back(Parking_Lot(test,pile_num));
        cout<<"老板真配合=w=(停车场成功建造。)\n";
        Sleep(3000);
        system("cls");
        return true;
	}
	else {
        cout << "该停车区域不满足建造条件，你这是在刁难建筑工人！" << endl;
        Sleep(3000);
        system("cls");
        return false;
    }
}

void Director::safeguard()/*补贴未实现*/
{
    is_running = 0; //停止营业
    ///将停车场的车辆都清出去
    cout<<"（四个猛男停车场的广播通知）很抱歉的提醒车主们，该停车场进入维修工作，你们需在两小时内离开四个猛男停车场，因为是突然的维修，所以不计停车费用。如造成不便，敬请原谅。\n\n\n";
    /*车主需要补贴什么的函数*/
    car_map.clear();    //把所有车都请出去
    Sleep(7000);
    system("cls");
    total_space = 0;
    for(int i=0;i<P.size();i++){
        P[i].Get_space_res() = 0;   //space_num不会改变，而res要重置为space_num
        for(int j=0;j<P[i].Get_concrete_vec().size();j++){
            for(int k=0;k<P[i].Get_concrete_vec()[j].size();k++){
                P[i].Get_space_res()++;
                total_space++;
                //puts("test1");
                P[i].Get_concrete_vec()[j][k].Space_clear();    //将车位清空
            }
        }
    }
    res_space = total_space;
    Parking_draw();//将停车场以及违法地区给画出来
    while(!(dijkstra_opt(0,P.size()+2,0)&&dijkstra_opt(100,P.size()+2,1))){
        cout<<"（管理者界面显示）当前四个猛男停车场不是一个连通图，不能营业！\n";
        cout<<"请您将某些停车场相连，使其构成连通图;\n";
        cout<<"温馨提示：最好先查看一下地图，看看要跟哪个区域相连哦~\n";
        cout<<"(是否要查看地图(0为可以建造，1为不可)？（y=1,n=0）)：";
        bool flagx;
        cin>>flagx;
        if(flagx){
            MAP_legal_show();
        }
        cout<<"请输入连通的两个点（入口是0，出口是100）（u，v）：";
        int u,v;
        while(cin>>u>>v&&(u==v||(u>P.size()&&u!=100)||(v>P.size()&&v!=100))){
            printf("u与v不能相同且要合法！重新输入：");
        }

        site buf1,buf2;
        if(u==0) buf1 = B;
        else if(u==100) buf1 = E;
        else buf1 = P[u-1].Return_Location();
        if(v==0) buf2 = B;
        else if(v==100) buf2 = E;
        else buf2 = P[v-1].Return_Location();
        //printf("test6:%d %d,%d %d\n",buf1.x,buf1.y,buf2.x,buf2.y);
        int w = bfs_opt(buf1,buf2); ///返回最短路径的长度（即有向图中的边权）
        insert2(u,v,w);
        cout<<"成功连通两地！\n";
        Sleep(1500);
        system("cls");
    }
    cout<<"四个猛男停车场的连通图已建成！\n\n\n";
    Sleep(2000);
}

void Director::show_info()    //查看停车场的相关信息
{
    printf("您好，这里是查看系统\n");
    if(P.size()!=0){
        printf("您目前有%d个停车场。您可以查询1~%d的停车场信息（输入100返回管理者系统）:",P.size(),P.size());
        int num;
        while(scanf("%d",&num)==1&&num!=100){
            if(num<1||num>P.size()){
                printf("非法输入！请重新输入数字（输入100为退出）：");
            }
            else{
                printf("---------------------------------------\n");
                printf("%-7s%d%s","第",num,"号停车场的信息\n");
                printf("***************************************\n");
                printf("%-7s%d%s","共有",P[num-1].Get_concrete_vec().size(),"层\n");
                for(int i=0;i<P[num-1].Get_concrete_vec().size();i++){
                    printf("第%d层的车位个数为：%d\n",i+1,P[num-1].Get_concrete_vec()[i].size());
                }
                printf("\n");
                printf("其与：");
                int flag = 1;
                for(int i=p[num];i!=-1;i=edge[i].next){
                    if(!flag) printf(" ");
                    flag = 0;
                    if(edge[i].v==0) printf("入口");
                    else if(edge[i].v==100) printf("出口");
                    else printf("%d号停车场",edge[i].v);
                }
                printf("相连\n");
                printf("***************************************\n");
                printf("---------------------------------------\n");
                printf("您还可以查询1~%d的停车场信息（输入100返回管理者系统）:",P.size());
            }
        }
    }
    else{
        printf("您现在没有一个停车场，没有信息可查！（返回管理者系统）\n");
    }
    system("cls");
    cout<<"正在返回主界面...\n";
    Sleep(3000);
    system("cls");
    show_manager_content();
    return ;
}

void Director::check_in()//司机操作
{
    if(is_running){
        cout<<"欢迎";
        if(res_space>0){    //都在director类里面，所以可以直接调用
            string s2;
            cout<<"（入口电子显示屏显示）您无需等待，可直接进入停车场入口办理手续。\n";
            cout<<"车牌：（虽然我们先进的四个猛男停车场有车牌扫描功能（假的），但你不由自主的输入了你的车牌号与车的品牌）";
            cout<<"车牌：";
            cin>>s2;
            Q.push(car(s2));
            succeed_in(1,make_pair(0,0),0);//后面两个参数无实际意义
            return ;
        }
        else{
            cout<<"（入口出票机屏幕显示）很抱歉的通知你，该停车场已满人；\n";
            cout<<"（入口出票机屏幕显示）目前的停车场排队人数：\n";
            cout<<'\n';
            big_number(Q.size());//显眼的大数显示，给司机明确的反馈
            cout<<'\n';
            cout<<"（入口出票机屏幕显示）您要选择等待，并将车辆停放在我们的四个猛男停车场吗？（输入1or0）\n";
            bool flagxx ;
            cin>>flagxx;
            if(flagxx){
                string s2;
                cout<<"好的。（虽然我们先进的四个猛男停车场有车牌扫描功能（假的），但你不由自主的输入了你的车牌号与车的品牌）";
                cout<<"车牌：";
                cin>>s2;
                Q.push(car(s2));
                cout<<"（入口出票机屏幕显示）谢谢您的耐心，这是你的车票：(你得到了一个纸质小票)\n";
                cout<<"\n\n";
                printf("%-5s\n","四个猛男停车场");
                printf("%-4s\n","停车申请");
                printf("------------------------\n");
                printf("车牌号：%s\n",s2.c_str());
                printf("停车路段：图强路\n"
                       "请您关好门窗，保管好随身携带贵重物品！\n"
                       "大连市四个猛男停车场大工建设管理有限公司\n"
                       "客服热线6666-520dlut\n");
                cout<<"\n\n";
            }
            else{
                cout<<"感谢您的到来，如有疑问，致电6666-520dlut\n";
            }
        }
    }
    else{
        printf("非常抱歉，四个猛男停车场当前没有营业。欢迎下次光临！如有疑问，致电6666-520dlut\n");
    }
}

/*最短路径的语言修饰还没写好*/
void Director::succeed_in(bool need_to_search,const PII &in_pile,const int &in_space)//停车场已经建好了，不需要调用dijk（同时这不是司机操作的）
{
    cout<<"(入口显示屏)车牌号："<<Q.front().number<<"，您已进入闸道\n";
    string space_NAME;
    int search_num;
    if(need_to_search){
        Space_search_and_insert(space_NAME,search_num);
    }
    else{
        search_num = in_pile.first;
        int i = in_pile.second,j = in_space;
        P[search_num].Parking_car(i,j,1,Q.front().number,time(0));/*之后传一些信息到车位上*/
        P[search_num].Get_concrete_vec()[i][j].Get_Begin_string() = printDate();//给发票提供信息
        space_NAME = P[search_num].space_NName(i,j);
        P[search_num].Red_space_res();
        res_space--;
        car_map[Q.front().number] = in_pile;
    }
    printf("智能导航为您服务，我们将为您选择最方便(路径最短)的停车路线\n");
	printf("您的推荐车位是%c号停车场,%s号车位\n推荐路线是：\n", 'A'+search_num, space_NAME.c_str());
	find_shortest_way(search_num+1,0);/*对最短路径的语言修饰*/

    Q.pop();
    Sleep(10000);
}

void Director::check_out()    //如果车队里不为空，让一辆车进入（这是司机操作的,隶属司机app界面）
{
    bool need_to_search = 1;
    string numx;
    cout<<"请告诉我您的车牌号，我将为您提供退场手续-w-：";
    cin>>numx;
    if(car_map.count(numx)){
        PII tmp = car_map[numx];
        car_map.erase(numx);    //请出
        int parking_NUMber = tmp.first,pile_Num = tmp.second;
        int in_space;
        for(int i=0;i<P[parking_NUMber].Get_concrete_vec()[pile_Num].size();i++){
            if(numx==P[parking_NUMber].Get_concrete_vec()[pile_Num][i].Get_car_number()){
                in_space = i;   ///出车的停车位，为队列不为空的情况提供信息
                double buf_pay = take_charge_rate * (time(0) - P[parking_NUMber].Get_concrete_vec()[pile_Num][i].Get_Begin_Time());
                bool is_price = 0;
                if(Lucky_number[0]!='\0' && Lucky_number == P[parking_NUMber].Get_concrete_vec()[pile_Num][i].Get_SpaceName())
                {
                    cout<<"Oh，该车位是幸运车位！将为你打五折优惠~~\n";
                    buf_pay /= 2.0;
                    is_price = 1;
                }
                Final_Income += buf_pay;
                printf("四个猛男停车场小票\n");
                printf("车牌号码:%s\n", numx.c_str());
                printf("停车场:%c号停车场\n", 'A' + parking_NUMber);
                printf("车位编号:%s\n", P[parking_NUMber].Get_concrete_vec()[pile_Num][i].Get_SpaceName().c_str());
                printf("停车起始时间：%s\n",P[parking_NUMber].Get_concrete_vec()[pile_Num][i].Get_Begin_string().c_str());
                printf("停车结束时间：%s\n",printDate().c_str());
                printf("总停车时间：%d秒\n",time(0) - P[parking_NUMber].Get_concrete_vec()[pile_Num][i].Get_Begin_Time());
                printf("是否中奖：%d(1为中奖)\n", is_price);
                printf("合计收款：%.2f\n", buf_pay);
                printf("谢谢您的光临，祝您出行愉快\n");
                printf("最快离开的路径：");
                find_shortest_way(parking_NUMber+1,1);
                P[parking_NUMber].Get_concrete_vec()[pile_Num][i].Space_clear();
                P[parking_NUMber].Get_space_res()++;
                res_space++;/*可能有bug*/
                break;
            }
        }
        Customer_Feedback();    //给退场的司机的用户反馈
        if(!Q.empty()){
            need_to_search = 0;
            succeed_in(need_to_search,tmp,in_space);
        }
    }
    else{
        cout<<"您的车不在我们四个猛男停车场里。请确认好信息后再进行操作\n";
        Sleep(3000);
        return ;
    }
    Sleep(5000);
}

void Director::big_number(int num)    ///队列的大小。用栈处理num，然后存入字符串输出
{
    stack<int> stack_num;
    char c[20] = {0};//初始化
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
    ///预处理 得到times与nums数组
    char buf_hor[24] = {0};
    buf_hor[0]= ' ';
    for(int i=1;i<=times;i++){
        buf_hor[i] = '-';
    }
    buf_hor[times+1] = ' ';
    ///横线
    for(int i=0;i<=9;i++){
        //if(i==1)continue;
        for(int j=0;j<=2*times+2;j++){
            for(int k=0;k<=times+1;k++){
                number_M[i].model[j][k] = ' ';
            }
        }
    }
    ///空格
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
    ///制模型
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

void Director::dijk_init()  //图论算法初始化
{
    eid = 0;
    memset(p,-1,sizeof(p));
}

void Director::insert1(int u,int v,int w)///一条由u指向v的有向边
{///当in_out等于0的时候就是计算入口，1为出口
        edge[eid].v = v;
        edge[eid].w = w;
        edge[eid].next = p[u];
        p[u] = eid++;
        flag_dijk = 1;  ///增边了
}

void Director::insert2(int u,int v,int w)///一条由u与v的无向边
{
    insert1(u,v,w);
    insert1(v,u,w);
}

void Director::delete_point(int s)    ///摧毁停车场s，同时删掉停车场对应的边 /*尚未投入应用中*/
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
    flag_dijk = 1;  ///删点了
}

bool Director::dijkstra_opt(int s,int n,bool in_out)    ///s代表入口。如果不是连通图，则返回0，作为非法的停车场管理做出判断
//n的个数应该是停车场个数+2，算上出口100，入口0
{///in_out=0是入口单源最短路，=1是出口单源最短路
    set<PII,less<PII> > min_heap;  ///用集合来伪实现一个小根堆，同时有映射二叉堆的功能。
    int vis[MAX_N];// 标记每个顶点是否在集合 U 中
    for(int i=0;i<MAX_N;i++){
        dist[i][in_out]=INF;
        new_road[i][in_out] = -1; ///为后面更新做好准备
        vis[i]=0;
    }
    min_heap.clear();
    ///数据初始化
    min_heap.insert(make_pair(0,s));
    dist[s][in_out] = 0;    ///初始点置为0
    for(int i=0;i<n;i++){
        if(min_heap.size()==0){// 如果小根堆中没有可用顶点，说明有顶点无法从源点到达，算法结束
            return false;
        }
        set<PII,less<PII> >::iterator it = min_heap.begin();
        int u = it->second;
        min_heap.erase(*it);
        vis[u] = true;
        // 进行和普通 dijkstra 算法类似的松弛操作
        for(int j=p[u];j!=-1;j=edge[j].next){
            int v = edge[j].v;
            if(!vis[v]&&dist[v][in_out]>dist[u][in_out]+edge[j].w){
                min_heap.erase(make_pair(dist[v][in_out],v));
                dist[v][in_out] = dist[u][in_out] + edge[j].w;
                new_road[v][in_out] = u;///记录路径
                min_heap.insert(make_pair(dist[v][in_out],v));
            }
        }
    }
    return true;
}

void Director::find_shortest_way(int s,bool in_out)   /*最短路径的语言修饰还没写好*/
{
///最后结合停车场的路，可以将输出修改一下。
    if(in_out==0){
        int t = 0;
        int way[100] = {0};
        way[t++] = s;
        while(new_road[s][0]!=-1){
            way[t++] = new_road[s][0];
            s = new_road[s][0];
        }
        way[t] = ENTRANCE;
        printf("ROADS:出口");
        for(int i=t-2;i>=0;i--){
            printf("--%d号停车场",way[i]);
        }
        printf("\n");
    }
    else{
        printf("ROADS:");
        while(new_road[s][1]!=-1){
            printf("%d号停车场--",s);
            s = new_road[s][1];
        }
        printf("出口\n");
    }
}

void Director::MAP_init()   //地图初始化
{
    memset(MAP,0,sizeof(MAP));
    memset(legal_map,0,sizeof(legal_map));
    for(int i=0;i<L;i++) for(int j=0;j<R;j++) MAPX[i][j] = '.'; //空白区域
}

void Director::Parking_draw() //停车场画在地图上
{
    int n = P.size();//停车场总数

    for(int i=0;i<9;i++) MAP[L-1][i] = MAP[L-2][i] = MAP[L-3][i] = -100,legal_map[L-1][i] = legal_map[L-2][i] = legal_map[L-3][i] = 1,MAPX[L-1][i] = MAPX[L-2][i] = MAPX[L-3][i] = '^';  //入口
    for(int i=R-1;i>R-10;i--) MAP[0][i] = MAP[1][i] = MAP[2][i] = -200,legal_map[0][i] = legal_map[1][i] = legal_map[2][i] = 1,MAPX[0][i] = MAPX[1][i] = MAPX[2][i] = '^';  //出口

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
                        if(is_road(Point(i,j,0))) {}    //如果是道路就跳过，免得重画路了
                        else MAP[i][j] = ILLEGAL_NUM,MAPX[i][j] = '!';    //非法区域
                    }
                    legal_map[i][j] = 1;    //等于1是已占用，即非法
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

void Director::MAP_legal_show() //可选地图展示
{
    for(int i=0;i<L;i++){
        for(int j=0;j<R;j++){
            printf("%c",MAPX[i][j]);
        }
        printf("\n");
    }
    printf("\n注释：.为空白区域，#为停车场出口,!为非法区域，i(1~9)为i号停车场所占位置;\n");
    printf("^是出入口，*是道路\n");
}

void Director::MAP_show()   //地图展示
{
    if(!is_project){
        cout<<"连地皮都没有，哪来的地图（生气）\n";
        return;
    }
    for(int i=0;i<L;i++){
        for(int j=0;j<R;j++){
            printf("%c",MAPX[i][j]);
        }
        printf("\n");
    }
    printf("\n注释：.为空白区域，#为停车场出口,!为非法区域，i(1~9)为i号停车场所占位置;\n");
    printf("^是出入口，*是道路\n");
    printf("是否要退出查看？按回车键退出(将会等待3秒):");
    getchar();
    getchar();
}

void Director::Parking_delete(){}  /*删除道路用dfs*/

bool Director::is_road(const Point &A)    //判断是否为路
{
    return MAP[A.x][A.y]>ILLEGAL_NUM&&MAP[A.x][A.y]<=20001;
}

void Director::find_start_points(const site &A,const site &B,Point &a1)   //A->B，将两个停车场的相对位置分成四种情况，判断方向，避免自动生成的路绕建筑一圈
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

bool Director::legal_for_road(const Point&A)  //允许铺路的地方。用于vis_bfs数组的初始化
{
    return (MAP[A.x][A.y]>=ILLEGAL_NUM&&MAP[A.x][A.y]<=20001)||MAP[A.x][A.y]==0;
}

void Director::dye_track(int u,int v,Point &finalx)    //路径染色，使用哈希映射，同时将染色值存入set集合中。
{
    int color_num = (u*10086+v*10010)%10000+10001;
    colx.insert(color_num);
    OP sta = op[finalx.ind];

    while(sta.pre!=-1){  //当反向链式前向星还没到头
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
    }   //初始化
}

int Director::bfs_opt(site A,site B)  //bfs铺路算法
{
    vis_init();
    if(B.number==0) B.number = -100;
    if(B.number==100) B.number = -200;//改变搜索编号
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
    }//vis初始化
    queue<Point> Q_bfs;
    while(!Q_bfs.empty()) Q_bfs.pop();
    Point start_p;
    int id=0;       ///为链式前向星做准备
    op[id++] = OP(start_p.x,start_p.y,-1);  //最初的点
    find_start_points(A,B,start_p);
    start_p.ind = 0;
    Q_bfs.push(start_p);
    //cout<<"test:"<<start_p.x<<" "<<start_p.y<<" "<<start_p.ind<<"\n";
    Point finalx;
    int cur = 0,last = 1,step = 0;  ///用作路径长度记录，很好理解就不介绍了。cur是现存的点，last是剩余的点，step代表步数
    while(!Q_bfs.empty()){
           // puts("TEST1-----------------");
        Point st = Q_bfs.front();
        int ID = st.ind;
        Q_bfs.pop();
        last--;
        int countx = 0;
        for(int i=rand()%4;countx<4;i++,i%=4,countx++){
            int tx = st.x + dir[i][0],ty = st.y + dir[i][1];
            if(!in(tx,ty)) continue;    //越界区域要跳过
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
    puts("错误TEST2_bfs-----------------（不应显示的错误）");
}

void Director::lottery_system()   //用降维遍历来优化算法
{
    if(!P.size()){
        cout<<"当前没有车位，不能设置幸运车位（将返回管理者界面）"<<"\n";
        return;
    }

    int ramx = rand()%total_space;   ///对所有车位
    int i=0,j=0,k=0;
    while(ramx>P[i].Get_num()){  //先快速找到它对应的车位，再逐个搜索
        ramx-=P[i].Get_num();
        i++;
    }
    while(ramx>P[i].Get_concrete_vec()[j].size()){  //先快速找到它对应的车位，再逐个搜索
        ramx-=P[i].Get_concrete_vec()[j].size();
        j++;
    }
    while(ramx>1){  //边界处理
        ramx--;
        k++;
    }
    Lucky_number = P[i].space_NName(j,k);
    printf("幸运车位的号码是：%s\n",Lucky_number.c_str());
    printf("当司机进入此幸运车位的时候，结账时可打五折\n");
    return ;
}

void Director::prime_game_system()  //游戏系统
{
	int begin = 0, end = 0, times = 1, clock = 0;
	int number, judge = -1, set = -1;
	int n = 0;
	int guess = -1;
	srand((int)time(NULL));
	number = rand() % 1000;
	cout << "游戏名称：猜数字游戏\n游戏规则：系统会在0到1000中随机生成一个数，你需要猜中这个数字是多少（每次您给出数字后我们会提示这个数字是偏大还是偏小）";
	cout << "，只要在30秒以内或者15次猜数之内猜中了系统生成的随机数，则你将获得胜利" << endl;
	cout << "输入以下序号以使用功能：\n1.开始游戏\n2.回到用户界面\n3.系统设置" << endl;
	cout << "请输入使用哪项功能：";
	cin >> judge;
	L1:while (1) {
		if (judge == 1)
			break;
		if (judge == 2)
			show_customer_content();
			system("cls");
			cout<<"将回到用户界面...\n";
			Sleep(3000);
			return ;
		if (judge == 3) {
			system("cls");
			cout << "输入序号以使用设置：\n1.修改随机数范围\n2.退出设置" << endl;
			cout << "请输入使用哪项设置：";
			cin >> set;
			judge = -1;
			while (1) {
				if (set == 1) {
					cout << "请设置随机数的范围为由0到N\n请输入N:";
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
		cout << "输入以下序号以使用功能：\n1.开始游戏\n2.退出系统\n3.系统设置" << endl;
		cout << "请输入使用哪项功能：";
		cin >> judge;
		system("cls");
	}
	cout << "随机数已经生成，游戏会在您输入第一个数字后计时\n请您猜一个数字：" << endl;
	cin >> guess;
	begin = (int)time(NULL);
	while (guess != number) {
		times++;
		system("cls");
		if (guess > number) {
			cout << "您给出的数字是" << guess << endl;
			cout << "您猜的数字太大了，请您重新再猜一个数字：" << endl;
		}
		else {
			cout << "您给出的数字是" << guess << endl;
			cout << "您猜的数字太小了，请您重新再猜一个数字：" << endl;
		}
		cin >> guess;
	}
	end = (int)time(0);
	clock = end - begin;
	cout << "在本次游戏中\n您总共消耗时间：" << clock << "秒" << endl;
	cout << "您总共尝试猜数的次数：" << times << "秒" << endl;
	if (clock <= 30 || times <= 15)
		cout << "Congratulations,you win!!!" << endl;
	else
		cout << "Sorry,you lose." << endl;
}

void Director::do_business()    //开始营业
{
    if(!P.size()){
        cout<<"连停车场都没有，不能开始营业！\n";
    }
    else{
        cout<<"开始营业\n";
        is_running = 1; //开始营业
    }

}

void Director::Customer_Feedback()    //用户调用
{
    printf("再次感谢您享受我们四个猛男停车场的服务。您有意向填写关于我们服务的意见反馈吗？1or0\n");
    bool buf;
    cin>>buf;
    string s1;
    if(buf){
        printf("您觉得可以给我们四个猛男停车场评多少分呢？（0~100）:");
        int mark;
        scanf("%d",&mark);
        printf("愿意留下您的意见吗？1or0:");
        int buf2;
        scanf("%d",&buf2);
        if(buf2){
            cout<<"您的意见是：";
            getchar();
            getline(cin,s1);
        }
        else{
            s1 = "\0";
        }
        Rank.get_in_feedback(mark,s1);
    }
    printf("感谢您的到来，欢迎下次光临\n");
    //show_main_content();不需要写
}

void Director::show_Ranked_list()
{
    if(Rank.Get_num()){
        Rank.Rank_show();
    }
    else{
        printf("目前尚没有车主给我们留下评价！\n");
    }
}

void Director::Space_search_and_insert(string &space_NAME,int &search_num)//提供给找回路径的信息
{
    int minn = INF; //让最小距离初始为极大值，然后枚举所有停车场，找到距离最近的可以泊车的停车场
    for(int i=0;i<P.size();i++){
        if(P[i].Get_space_res()>0){
            if(minn>dist[i+1][0])   ///dist[i+1][0]代表第i+1号停车场到入口0的最短距离
            {
                minn = dist[i+1][0];
                search_num = i;//一定有解，根据数论中的抽屉原理，必然存在一个停车场的剩余车位大于0
            }
        }
    }/*有问题*/
    for(int i=0;i<P[search_num].Get_concrete_vec().size();i++){
        for(int j=0;j<P[search_num].Get_concrete_vec()[i].size();j++){
            if(P[search_num].Get_concrete_vec()[i][j].Get_State()==0){
                P[search_num].Parking_car(i,j,1,Q.front().number,time(0));/*之后传一些信息到车位上*/
                P[search_num].Get_concrete_vec()[i][j].Get_Begin_string() = printDate();//给发票提供信息
                space_NAME = P[search_num].space_NName(i,j);
                P[search_num].Red_space_res();//停车场车位减少
                res_space--;//总车位也要减少
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
    sprintf(s1,"%d年%d月%d日%d时%d分%d秒\n",year,month,day,hour,minute,second);
    string s2 = s1;
    return s2;
}

void Director::Class_Test()
{
    cout<<"目前没有需要测试的地方\n";
}

void Director::change_charge()
{
    cout<<"你想将收费系数改成多少呢？（合适推荐0.05~0.5）\n";
    cin>>take_charge_rate;
    cout<<"修改完成\n";
}

void Director::show_money()
{
    cout<<"目前营额为："<<Final_Income<<'\n';
}

void Director::ADD_TRACK()
{
    cout<<"(是否要查看地图(0为可以建造，1为不可)？（y=1,n=0）)：";
    bool flagx;
    cin>>flagx;
    if(flagx){
        MAP_legal_show();
    }
    cout<<"请输入连通的两个点（入口是0，出口是100）（u，v）：";
    int u,v;
    while(cin>>u>>v&&(u==v||(u>P.size()&&u!=100)||(v>P.size()&&v!=100))){
        printf("u与v不能相同且要合法！重新输入：");
    }

    site buf1,buf2;
    if(u==0) buf1 = B;
    else if(u==100) buf1 = E;
    else buf1 = P[u-1].Return_Location();
    if(v==0) buf2 = B;
    else if(v==100) buf2 = E;
    else buf2 = P[v-1].Return_Location();
    //printf("test6:%d %d,%d %d\n",buf1.x,buf1.y,buf2.x,buf2.y);
    int w = bfs_opt(buf1,buf2); ///返回最短路径的长度（即有向图中的边权）
    insert2(u,v,w);
    dijkstra_opt(0,P.size()+2,0);
    dijkstra_opt(100,P.size()+2,1);
    cout<<"成功连通两地！\n";
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
