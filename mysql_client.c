#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <unistd.h>  //STDOUT_FILENO
#include <math.h>
#include "mysql.h"

int main()
{
    //MYSQL *mysql_init(MYSQL *mysql)
    //mysql初始化
    MYSQL *mysql = mysql_init(NULL);
    if( mysql == NULL )
    {
        printf("mysql init error");
    }
    printf("mysql init successful\n");

    //连接mysql数据库
    //MYSQL *mysql_real_connect(MYSQL *mysql,               初始化结果
    //                          const char *host,           ip地址
    //                          const char *user,           用户名
    //                          const char *passwd,         密码
    //                          const char *db,             数据库名
    //                          unsigned int port,          端口号 0为默认窗口 若不为0，其值将用作于TCP/IP连接的端口号
    //                          const char *unix_socket,    若不是NULL，该字符串描述了应使用的套接字或命名管道
    //                          unsigned long client_flag)  通常传 0 
    //  conn == mysql
    MYSQL *conn = mysql_real_connect(mysql,"localhost", "root", "000","test",0,NULL,0);
    if(conn == NULL)
    {
        printf("mysql connect error , [%s]\n",mysql_error(mysql));
    }
    printf("mysql connect successful\n");

    if( conn = mysql )
    {
        printf("conn and mysql is same\n");
    }

    //获取当前使用的字符集
    printf("before---%s---\n",mysql_character_set_name(conn));

    //设置默认使用的字符集
    mysql_set_character_set(conn,"utf8");

    //获取修改之后的字符集
    printf("after---%s---\n",mysql_character_set_name(conn));

    int i;
    int ret;
    char buf[1024];

    while(1)
    {
        //打印提示符
        write(STDOUT_FILENO,"mysql> ",strlen("mysql> "));

        //接受用户输入
        memset( buf , 0x00 , sizeof(buf) );//memset()为初始化函数 将数组全部初始化为某值
        read( STDIN_FILENO , buf , sizeof(buf) ); //读取输入的数据

        buf[ strlen(buf) - 1 ] = '\0'; //将最后的回车去除

        char *p = strrchr(buf,';');//将buf中的分号去除
        if( p != NULL )
        {
            *p = '\0';
        }

        int len = strlen(buf);

        //过滤buf之前的空格
        for (i=0; i<len ;i++)
        {
            if(buf[i] == ' ')
            {
                continue;//继续for循环
            }
            else
            {
                break;//跳出for循环
            }
        }
        memmove(buf, buf+i,len-i);//去除空格后放入buf中

        buf[len-i] = '\0';
        printf("buf==[%s]\n",buf);

        //如果输入字符为 exit 或者 quit 则退出
        if(strncasecmp(buf, "exit", 4)==0 || strncasecmp(buf, "quit", 4)==0 )
        {
            mysql_close(conn);
            exit(0);
        }

        //执行sql语句
        ret = mysql_query(conn, buf);
        if(ret != 0)
        {
            printf("mysql_query error , [%s]\n",mysql_error(conn));
            continue;
        }

        //若用户输入的不是select查询，则打印影响的行数
        if(strncasecmp(buf, "select", 6) != 0 )
        {
            printf("Query OK, %d row affected\n", mysql_affected_rows(conn));
            continue;
        }

        //下面是select查询情况
        //获取结果集
        MYSQL_RES *results = mysql_store_result(conn);
        if( results == NULL )
        {
            printf("mysql_store_result error , [%s]\n",mysql_error(conn));
            continue;
        }

        //获取列数
        int num = mysql_num_fields(results);

        //获取表头信息
        MYSQL_FIELD *field = mysql_fetch_fields(results);
        if( field == NULL )
        {
            printf("mysql_fetch_fields error , [%s]\n",mysql_error(conn));
            mysql_free_result(results);//释放结果集
            continue;
        }

        //打印表头信息
        printf("+-----+-------+\n");
        for( i=0 ; i<num ;i++)
        {
            printf("  %s  ",field[i].name);
        } 
        printf("\n+-----+-------+\n");

        //循环获取每一条记录
        MYSQL_ROW row;
        while( row = mysql_fetch_row(results) )
        {
            for ( i= 0; i < num; i++)
            {
                printf("   %s  ",row[i]);
            }
            printf("\n");
        }
        //释放结果集
        mysql_free_result(results);
    }

    //释放资源
    mysql_close(mysql);

    return 0;
}