#include <mysql/mysql.h>
#include <stdio.h>
#include <iostream>
#include <string>
// mysql -u debian-sys-maint -puUakoE1X8Ei6TqLJ
const char *db_host = "host.docker.internal";
const char *db_user = "debian-sys-maint";
const char *db_pass = "uUakoE1X8Ei6TqLJ";
const char *db_name = "data";
const int db_port = 3306;

int main() {
    MYSQL *mysql = mysql_init(NULL);  //初始化数据库连接变量
    if (mysql == NULL) {
        printf("Error:%s\n", mysql_error(mysql));
        exit(1);
    }
    mysql = mysql_real_connect(mysql, db_host, db_user, db_pass, db_name, db_port, NULL,
                               0);  //连接mysql
    if (mysql)
        printf("Success!\n");  //连接成功
    else
        printf("Failed!\n");

    //建立好链接之后，获取英文没有问题，但如果获取中文就是乱码。
    //设置链接的默认字符集为utf8，原始默认是latin1(拉丁文)
    mysql_set_character_set(mysql, "utf8");
    std::string sql = "select * from score";
    int n = mysql_query(mysql, sql.c_str());

    MYSQL_RES *res = mysql_store_result(mysql);
    if (res == nullptr) exit(0);

    int rows = mysql_num_rows(res);
    std::cout << "table has " << rows << " rows" << std::endl;

    int cols = mysql_num_fields(res);
    std::cout << "table has " << cols << " cols" << std::endl;

    MYSQL_FIELD *col_name = mysql_fetch_fields(res);
    for (int i = 0; i < cols; i++) {
        std::cout << col_name[i].name << "  |  ";
    }
    std::cout << std::endl;

    MYSQL_ROW line;                 //创建MYSQL_ROW对象，用来保存表中内容
    for (int i = 0; i < rows; i++)  //行数rows通过mysql_num_rows()获取。
    {
        line = mysql_fetch_row(res);
        for (int j = 0; j < cols; j++)  //列数cols通过mysql_num_fields()获取。
        {
            std::cout << line[j] << " ";  //打印第i行的第j列内容。
        }
        std::cout << std::endl;
    }

    // free(res); // 不需要自己释放指针了
    return 0;
}