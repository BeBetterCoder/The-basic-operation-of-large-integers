#define _CRT_SECURE_NO_WARNINGS 1；
#include<stdio.h>
#include<string.h>
#include <math.h>
#define MAX_SIZE 2000

//  实现大数运算的基本思路就是先用字符串实现四则运算，再用四则运算实现模逆、gcd等运算
//  环境:Visual Studio 2019

// maxer函数 判断str1 是否 大于str2
int maxer(char* str1, char* str2)
{
    int len1 = strlen(str1);
    int len2 = strlen(str2);
    if (len1 > len2)
        return 1;
    else if (len1 < len2)
        return -1;
    else
    {
        int n = strcmp(str1, str2);
        if (n > 0)
            return 1;
        else if (n == 0)
            return 0;
        else
            return -1;
    }
}

//  加法
char* add(char* str1, char* str2)
{   
    //  确定较大，较小的字符串，并获取它们的长度
    int n = maxer(str1, str2);
    char* max_str = n >= 0 ? str1 : str2;
    char* min_str = n >= 0 ? str2 : str1;
    int len_max = strlen(max_str);
    int len_min = strlen(min_str);

    //  创建 较大字符串 长度大1 的空间 存储结果
    char* str = malloc(MAX_SIZE + 1);
    str[len_max + 1] = '\0';

    //  进行加法运算
    int plus = 0;        // plus 为 进位
    int i = len_max - 1; // 标记 较大字符串 最低位字符的位置
    int j = len_min - 1; // 标记 较小字符串 最低位字符的位置
    for (; i >= 0; i--)
    {
        if (j >= 0)
        {
            str[i + 1] = max_str[i] + min_str[j] - '0' + plus; // 字符相加
            j--;                                               // 位置左移 1
            plus = str[i + 1] > '9' ? 1 : 0;                   // 判断是否进位
            if (plus == 1)
                str[i + 1] -= 10; // 进位 则减 10
        }
        else // 较小字符串的字符 全部加完
        {
            str[i + 1] = max_str[i] + plus;
            plus = str[i + 1] > '9' ? 1 : 0;
            if (plus == 1)
                str[i + 1] -= 10;
        }
    }

    //  根据 最高位 是否 进位，确定 返回指针
    if (plus == 0)
        return str + 1; // 返回 str 右移1位的位置
    else
    {
        str[0] = '1'; // str首位 赋 '1'
        return str;   // 返回 str
    }
}
 
//  减法
char* sub(char* str1, char* str2){

//  确定结果 正负 或 0
int is_neg = maxer(str1, str2);
if (is_neg == 0)
return "0"; // 若为 0, 直接返回 "0" 结束

char* max_str = is_neg > 0 ? str1 : str2;
char* min_str = is_neg > 0 ? str2 : str1;
int len_max = strlen(max_str);
int len_min = strlen(min_str);

char* str = malloc(MAX_SIZE + 1);
str[len_max + 1] = '\0';

//  进行减法运算
int plus = 0; // plus 为退位
int i = len_max - 1;
int j = len_min - 1;
for (; i >= 0; i--)
{
    if (j >= 0)
    {
        str[i + 1] = max_str[i] - min_str[j] + '0' + plus; // 字符相减
        j--;
        plus = str[i + 1] < '0' ? -1 : 0; // 是否 退位
        if (plus == -1)
            str[i + 1] += 10; // 退位 则加 10
    }
    else
    {
        str[i + 1] = max_str[i] + plus;
        plus = str[i + 1] < '0' ? -1 : 0;
        if (plus == -1)
            str[i + 1] += 10;
    }
}

//  确定最后一个 前位0 的位置，默认 str首位 为'0'
int n_zero = 0;
for (int i = 1; i <= len_max; i++)
{
    if (str[i] != '0')
        break;
    n_zero++;
}

//  根据 正负 和 最后一个前位0的位置 确定 返回指针
if (is_neg < 0)
{
    str[n_zero] = '-';   // 将最后一个前置0的 赋为 '-'
    return str + n_zero; // 返回'-'的位置
}
else
return str + n_zero + 1; // 返回最后一个前置0 右移1位的位置
}


//  乘法
char* mul(char* str1, char* str2)
{
    // 判断 str1 和 str2 中是否为 0,若有, 返回 "0" 结束
    if (*str1 == '0' || *str2 == '0')
        return "0";

    // 提高效率
    int n = maxer(str1, str2);
    char* max_str = n > 0 ? str1 : str2;
    char* min_str = n > 0 ? str2 : str1;
    int len_min = strlen(min_str);

    // 进行乘法运算
    char* str_sum = "0";
    // str2每个字符 乘 str1
    for (int i = 0; i < len_min; i++)
    {
        char* str = "0";
        int m = (int)(min_str[i] - '0'); // 把字符转成整型
        if (m == 0)
            continue;
        else
        {
            for (int j = 0; j < m; j++)
                str = add(str, max_str);
        }
        // 根据字符位数 往str后面 添 '0'
        int len = strlen(str);
        for (int j = 0; j < len_min - i - 1; j++)
        {
            str[len + j] = '0';
        }
        str[len + len_min - i - 1] = '\0';
        //printf("%s\n", str);
        // 将每次得到的结果相加
        str_sum = add(str_sum, str);
        //printf("%s\n", str_sum);
    }
    return str_sum;
}

// 取模
char* mol(char* str1, char* str2)
{
    if (*str2 == '0')
        return " 0 不能作为除数！";
    if (maxer(str1, str2) == -1)
        return str1;
    char* str_mol = add(str1, "0");
    char* str_divi = add(str2, "0");
    char* str_sum = "0";
    int n = strlen(str1) - strlen(str2);
    char* str_n = "1";
    for (int i = 0; i < n; i++)
        str_n = mul(str_n, "10");
    str_divi = mul(str_divi, str_n);
    for (int i = 0; i <= n; i++)
    {
        while (maxer(str_mol, str_divi) >= 0)
        {
            str_mol = sub(str_mol, str_divi);
            str_sum = add(str_sum, str_n);
        }
        if (i < n)
        {
            *(str_n + strlen(str_n) - 1) = '\0';
            *(str_divi + strlen(str_divi) - 1) = '\0';
        }
    }
    return str_mol;

}

//  除法 用于求模逆和公因子
char* divi(char* str1, char* str2)
{
    // 解决一些特殊情况
    if (*str2 == '0')
        return " 0 不能作为除数！";
    if (maxer(str1, str2) == -1)
        return "0";

    // 初始化 余数(被除数) 除数 和 结果
    char* str_mol = add(str1, "0");  // 得到 str1 副本
    char* str_divi = add(str2, "0"); // 得到 str2 副本
    char* str_sum = "0";

    // 进行除法运算
    // 将 str_mol 和 str_divi 右对齐
    int n = strlen(str1) - strlen(str2);
    char* str_n = "1";
    for (int i = 0; i < n; i++)
        str_n = mul(str_n, "10");    // 得到str_divi 扩大的倍数
    str_divi = mul(str_divi, str_n); // 实现右对齐

    for (int i = 0; i <= n; i++)
    {
        while (maxer(str_mol, str_divi) >= 0)
        {
            str_mol = sub(str_mol, str_divi);
            str_sum = add(str_sum, str_n); // 将每次结果加起来
            //puts(str_mol);
            //puts(str_sum);
        }
        if (i < n)
        {
            *(str_n + strlen(str_n) - 1) = '\0';       // 去除尾位置的 '0'
            *(str_divi + strlen(str_divi) - 1) = '\0'; // 去除尾位置的 '0'
        }
    }
    return str_sum;
}


//  开方，牛顿迭代法
int mySqrt(char* x) {

    
    char* res = malloc(MAX_SIZE );
    char* last = malloc(MAX_SIZE ); 
    res = "1";
    last = "0";
    while ( maxer(res,last) ) //循环退出的条件是真正解与近似解的误差进入允许的范围
    {
        last = res;
        res = divi(add(res, divi(x, res)),"2");
         //迭代公式
    }
    return res;
}

//  欧几里得算法求最大公因子
char* gcd(char* a, char* b)
{
    char* tmp = malloc(MAX_SIZE);

  
        while (maxer(b,"0"))
        {
            tmp = mol(a,b);
            /*printf(tmp);
            printf("\n");*/
            a = b;
            b = tmp;
        }
        return a;
    }



// 求f mod d的逆元
void Extend_Euclid(char* a, char* b)
{
    char* R = a;
    char* S = "1";
    char* T = "0";
    char* R1 = b;
    char* S1 = "0";
    char* T1 = "1";
    char* q;
    char* t1;
    char* t2;
    char* t3;
    while (strcmp(R1,"0")!=0)
    {
       
        q = divi(R , R1);
      /*  printf(q);
        printf("\n");*/

       

        t1 = sub(R , mul(q , R1));
        t2 = sub(S , mul(q , S1));
        t3 = sub(T , mul(q , T1));
 
        R = R1;
        S = S1;
        T = T1;
        R1 = t1;
        S1 = t2;
        T1 = t3;
        
      
    }
    printf("逆元为：");
        printf(T);
    
}


//  测试基本运算
void test() {
    char* t1;
    char* t2;
    char* t3;
    char* t4;
    char* t5;
    char* t6;



    t1 = add("12123329292929292929229", "410101010116470");
    printf(t1);
    printf("\n");
    t2 = sub("2239398232830", "234848838348431");
    printf(t2);
    printf("\n");
    t3 = mul("12123329292929292929229", "410101010116470");
    printf(t3);
    printf("\n");
    t4 = mol("121233", "410101010116470");
    printf(t4);
    printf("\n");
    t5 = mySqrt("12123329292929292929229");
    printf(t5);
    printf("\n");
    t6 = gcd("18238380", "270");
    printf(t6);
    printf("\n");
    Extend_Euclid("12232455323232373", "2");
   

}



int main() {
    test();
    
    }