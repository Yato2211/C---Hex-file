int printSelect(void){
    char select = 'S';
    fflush(stdin);
    select = getchar();
    if ( select == 'S' || select == 's') return 1;
    return 0;
}