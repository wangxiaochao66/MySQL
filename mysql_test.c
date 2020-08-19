#include <stdio.h>
#include <stdlib.h>
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

    //执行sql语句 添加数据
    // char *pSQL = "insert into test_table values(2,'two');";
    // int ret = mysql_query(conn,pSQL);
    // if(ret != 0)
    // {
    //     printf("mysql connect error , [%s]\n",mysql_error(conn));
    // }

    //执行sql语句 减少数据
    // char *pSQL = "delete from test_table where name = 'two';";
    // int ret = mysql_query(conn,pSQL);
    // if(ret != 0)
    // {
    //     printf("mysql connect error , [%s]\n",mysql_error(conn));
    // }

    //执行sql语句 获取整个表数据
    char *pSQL2 = "select * from test_table";
    int ret2 = mysql_query(conn,pSQL2);
    if(ret2 != 0)
    {
        printf("mysql_query error , [%s]\n",mysql_error(conn));
    }

    //获取结果集
    MYSQL_RES *results = mysql_store_result(conn);
    if( results == NULL )
    {
        printf("mysql_store_result error , [%s]\n",mysql_error(conn));
    }

    //获取列数
    int num = mysql_num_fields(results);

    //获取表头信息
    MYSQL_FIELD *field = mysql_fetch_fields(results);
    if( field == NULL )
    {
        printf("mysql_fetch_fields error , [%s]\n",mysql_error(conn));
    }

    int i;
    //打印表头信息
    printf("+-----+-------+\n");
    for( i=0 ; i<num ;i++)
    {
        printf("  %s  ",field[i].name);
    } 
    printf("\n+-----+-------+\n");

    // //循环获取每一条记录 方式一
    // MYSQL_ROW row;
    // while( row = mysql_fetch_row(results) )
    // {
    //     printf("%s %s \n",row[0],row[1]);
    // }

    //循环获取每一条记录 方式二
    MYSQL_ROW row;
    while( row = mysql_fetch_row(results) )
    {
        for ( i= 0; i < num; i++)
        {
            printf("   %s  ",row[i]);
        }
        printf("\n");
    }

    //释放资源
    mysql_close(mysql);

    return 0;
}
