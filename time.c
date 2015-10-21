//
//  time.c
//  test
//
//  Created by mr.ji on 15/4/28.
//  Copyright (c) 2015年 mr.ji. All rights reserved.
//

#include <stdio.h>
#include <time.h>

int main ()
{
    time_t rawtime;
    struct tm * timeinfo;
    
    time ( &rawtime );
    timeinfo = localtime ( &rawtime );
    printf ( "\007The current date/time is: %s", asctime (timeinfo) );
    return 0;
}