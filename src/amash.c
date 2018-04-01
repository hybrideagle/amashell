#include "amash.h"
#include <string.h>
#include <time.h>

bool starts_with(const char *pre, const char *str)
{
        while (*pre != '\0')
        {
                if (*(pre++) != *(str++))
                {
                        return false;
                }
        }
        return true;
}

char prompt[200];

void generatePrompt()
{
        char cwd[100];
        char login[100];
        char hostname[100];
        getcwd(cwd, sizeof(cwd));
        getlogin_r(login, sizeof(login));
        gethostname(hostname, sizeof(hostname));
        printf(KBLU "\n%s@%s",hostname,login);
        printf(KRED ":");
        printf(KWHT "%s",cwd);
        printf(KRED "$");
        printf(KGRN "");
}

bool line_ends_with_slash(char *line)
{
        int offset = strlen(line) - 1;

        while (offset >= 0)
        {
                switch (line[offset])
                {
                case ' ':
                        line[offset] = '\0';
                        break;

                case '/':
                        line[offset] = '\0';
                        return true;

                default:
                        return false;
                }
                printf("Loop\n");
        }
        return false;
}

int count_blocks(char* input)
{
        char* i = input;
        int count = 0;
        int c=0;
        //printf("Length = %d\n",strlen(input));
        for(c=0; c<strlen(input); c++)
        {
                //printf("{%c}\n",i[0]);
                if(i[0]=='|')
                {
                        count++;
                        //printf("Match\n");
                }
                i++;
        }
        return count+1;
}

int count_sc(char* input)
{
        char* i = input;
        int count = 0;
        int c=0;
        //printf("Length = %d\n",strlen(input));
        for(c=0; c<strlen(input); c++)
        {
                //printf("{%c}\n",i[0]);
                if(i[0]==';')
                {
                        count++;
                        //printf("Match\n");
                }
                i++;
        }
        return count+1;
}

int offset = 0;
int offset_sc = 0;

char* extract(char* input)
{
        char* i = input+offset;
        //printf("offset = %d\nNEW : %s\n",offset,i);
        int j = 0;
        char block[100];
        int x = 0;
        for(x=0; x<100; x++)
        {
                block[x]='\0';
        }
        while(1)
        {
                if( (i[0]!='|') & (i[0]!='\0') & (i[0]!='\n') )
                {
                        //printf("{%c}\n",i[0]);
                        block[j]=i[0];
                        j++;
                        i++;
                }
                else
                {
                        offset=offset+j+1;
                        //printf("j = %d\n",j);
                        //printf("off = %d\n__________________________\n",offset);
                        return(strdup(block));
                }
                //sleep(1);
        }
}

char* extract_sc(char* input)
{
        char* i = input+offset_sc;
        //printf("offset = %d\nNEW : %s\n",offset,i);
        int j = 0;
        char block[100];
        int x = 0;
        for(x=0; x<100; x++)
        {
                block[x]='\0';
        }
        while(1)
        {
                if( (i[0]!=';') & (i[0]!='\0') & (i[0]!='\n') )
                {
                        //printf("{%c}\n",i[0]);
                        block[j]=i[0];
                        j++;
                        i++;
                }
                else
                {
                        offset_sc=offset_sc+j+1;
                        //printf("j = %d\n",j);
                        //printf("off = %d\n__________________________\n",offset);
                        return(strdup(block));
                }
                //sleep(1);
        }
}


Executable* segment(char* input)
{
        printf("\n");
        char temp[100];
        int match = 0;
        char* exe;
        char* argv[10];
        int argc = 0;
        char* stdin;
        char* stdout;
        //Check stdin
        char* i = input;
        int j = 0;
        int c;
        int x = 0;
        for(x=0; x<100; x++)
        {
                temp[x]='\0';
        }
        for(c=0; c<strlen(input); c++)
        {
                //printf("{%c}\n",i[0]);
                if( (i[0]=='>') | (i[0]=='\n') )
                {
                        //printf("UnMatch1\n");
                        match = 0;
                }
                if( (j>0) & (i[0]==' '))
                {
                        //printf("UnMatch2\n");
                        match = 0;
                }
                if(match==1)
                {
                        if(i[0]!=' ')
                        {
                                //printf("Copy\n");
                                temp[j] = i[0];
                                j++;
                        }
                }
                if(i[0]=='<')
                {
                        match = 1;
                        //printf("Match\n");
                }
                i++;
        }
        if(offset>0)
        {
                stdin = strdup("1\0");
        }
        else
        {
                stdin = strdup(temp);
        }
        printf("StdIn = %s\n",temp);
        x = 0;
        for(x=0; x<100; x++)
        {
                temp[x]='\0';
        }
        //Check stdout
        i = input;
        j = 0;
        match = 0;
        for(c=0; c<strlen(input); c++)
        {
                //printf("{%c}\n",i[0]);
                if( (i[0]=='<') | (i[0]=='\n') )
                {
                        //printf("UnMatch1\n");
                        match = 0;
                }
                if( (j>0) & (i[0]==' '))
                {
                        //printf("UnMatch2\n");
                        match = 0;
                }
                if(match==1)
                {
                        if(i[0]!=' ')
                        {
                                //printf("Copy\n");
                                temp[j] = i[0];
                                j++;
                        }
                }
                if(i[0]=='>')
                {
                        match = 1;
                        //printf("Match\n");
                }
                i++;
        }
        stdout = strdup(temp);
        printf("StdOut = %s\n",temp);
        for(x=0; x<100; x++)
        {
                temp[x]='\0';
        }
        i = input;
        j = 0;
        match = 1;
        int top = 0;
        for(c=0; c<strlen(input); c++)
        {
                if((i[0]=='<') | (i[0]=='>'))
                {
                        match = 0;
                        top = 1;
                        temp[j++] = ' ';
                }
                if((i[0]!='<')&(i[0]!='>')&(i[0]!=' ')&(top==1)&(match==0))
                {
                        top = 0;
                }
                if(match==1)
                {
                        //printf("Copy\n");
                        temp[j] = i[0];
                        j++;
                }
                if( ((match==0)&(top==0)&(i[0]==' ')) | ((match==0)&(top==0)&(i[0]=='<')) | ((match==0)&(top==0)&(i[0]=='>')) )
                {
                        match = 1;
                }
                i++;
        }
        printf("*Execute = %s\n",temp);
        int flag = 1;
        char arg[15];
        match=0;
        int count = 0;
        for(c=0; c<strlen(temp); c++)
        {
                //printf("{%c}\n",temp[c]);
                if((temp[c]==' ')|(temp[c]=='\n'))
                {
                        match = 0;
                }
                else
                {
                        match = 1;
                        flag = 0;
                }
                if((match==1)&(temp[c]!=' '))
                {
                        //printf("Match\n");
                        arg[count++]=temp[c];
                }
                if((match==0) & (flag==0))
                {
                        //printf("argc = %d\n",argc+1);
                        arg[count++]='\0';
                        argv[argc] = strdup(arg);
                        argc++;
                        flag = 1;
                        for(x=0; x<15; x++)
                        {
                                arg[x]='\0';
                        }
                        count = 0;
                }
        }
        if(strlen(arg)>0)
        {
                //printf("argc = %d\n",argc+1);
                argv[argc] = strdup(arg);
                argc++;
        }
        printf("argc = %d\n",argc);
        printf("argv = ");
        for(c=0; c<argc; c++)
        {
                printf("%s ",argv[c]);
        }
        printf("\n");
        exe = strdup(argv[0]);
        printf("Exec = %s\n",exe);
        Executable * e = new_executable();
        strcpy(e->exec_path,exe);
        e->stdin = stdin;
        e->stdout = stdout;
        e->stderr_to_stdout = true;
        for(c=0;c<argc;c++)
        {
          e->argv[c] = argv[c];
        }
        e->argc = argc;
        return e;
}

void parser(char* input)
{
        printf("Piped_Count = %d\n",count_blocks(input));

        int c = count_blocks(input);
        int i;
        for(i=0; i<c; i++)
        {
                printf("_____________________________\n\n");
                char *s = extract(input);
                //printf(">>%s\n",s);
                Executable * e = new_executable();
                e = segment(s);                                          //INDIVIDUAL EXECUTABLES : Each '|' seperated section
                //TODO Combine these executables to Parsed
        }
        offset = 0;
}

int run_event_loop()
{
        char *input = malloc(sizeof(char) * 1000);

        strcpy(input, "");
        while (true)
        {
                bool continued = false;
                generatePrompt();
                do
                {
                        continued = false;
                        char *line = readline(prompt);
                        printf("Line : %s\n",line);
                        if (line_ends_with_slash(line))
                        {
                                continued = true;
                        }
                        strcat(input, line);
                } while (continued == true);
                //log_debug("Read input '%s'", input);
                printf("Input : %s\n",input);
                if (!input)
                {
                        do_quit(NULL);
                }

                //add_history(input);
                printf("Parse\n");

                //ParsedInput *e = parse(input);

                //Parsing Begins
                printf("Count = %d\n",count_blocks(input));

                int c = count_sc(input);
                int i;
                for(i=0; i<c; i++)
                {
                        printf("_____________________________\n\n");
                        char *s = extract_sc(input);
                        printf(">>%s\n",s);
                        parser(s);                    //Each ';' seperated section
                }
                offset_sc = 0;
                //Parsing Ends

                //exec(e);

                free(input);
        }
        return 0;
}

int main()
{
        run_event_loop();
}
