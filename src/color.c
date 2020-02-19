#include<stdio.h>
#include"color.h"


void red ()
{
  printf("\033[1;31m");
}

void yellow()
{
  printf("\033[01;33m");
}

void blue()
{
  printf("\033[1;34m");
}

void green()
{
  printf("\033[1;32m");
}

void reset () 
{
  printf("\033[0m");
}
