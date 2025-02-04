#ifndef _STRBUF_H
#define _STRBUF_H

#include <ctype.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>
struct strbuf {
  int len;     //当前缓冲区（字符串）长度
  int alloc;   //当前缓冲区（字符串）容量
  char *buf;   //缓冲区（字符串）
};

void strbuf_init(struct strbuf *sb, size_t alloc);
void strbuf_attach(struct strbuf *sb, void *str, size_t len, size_t alloc);
void strbuf_release(struct strbuf *sb);
void strbuf_swap(struct strbuf *a, struct strbuf *b);
char *strbuf_detach(struct strbuf *sb, size_t *sz);
int strbuf_cmp(const struct strbuf *first, const struct strbuf *second);
void strbuf_reset(struct strbuf *sb);

void strbuf_grow(struct strbuf *sb, size_t extra);
void strbuf_add(struct strbuf *sb, const void *data, size_t len);
void strbuf_addch(struct strbuf *sb, int c);
void strbuf_addstr(struct strbuf *sb, const char *s);
void strbuf_addbuf(struct strbuf *sb, const struct strbuf *sb2);
void strbuf_setlen(struct strbuf *sb, size_t len);
size_t strbuf_avail(const struct strbuf *sb);
void strbuf_insert(struct strbuf *sb, size_t pos, const void *data, size_t len);

void strbuf_rtrim(struct strbuf *sb);
void strbuf_ltrim(struct strbuf *sb);
void strbuf_remove(struct strbuf *sb, size_t pos, size_t len);

ssize_t strbuf_read(struct strbuf *sb, int fd, size_t hint);
int strbuf_getline(struct strbuf *sb, FILE *fp);

struct strbuf **strbuf_split_buf(const char *str, size_t len, int terminator, int max);
bool strbuf_begin_judge(char *target_str, const char *str, int strnlen);
char *strbuf_get_mid_buf(char *target_buf, int begin, int end, int len);
#endif
//Part 2A
void strbuf_init(struct strbuf *sb, size_t alloc){
    sb->buf=(char*)malloc(alloc);
    sb->alloc=alloc;
    sb->buf[alloc]='\0';
}
void strbuf_attach(struct strbuf *sb, void *str, size_t len, size_t alloc){
    if(sb->buf!=NULL){
        free(sb->buf);
    }
    sb->buf=(char*)malloc(alloc);
    memcpy(sb->buf,str,len);
    sb->alloc=alloc;
    sb->len=len;
}
void strbuf_release(struct strbuf *sb){
    free(sb->buf);
    sb->buf=NULL;
    sb->len=0;
    sb->alloc=0;
}
void strbuf_swap(struct strbuf *a, struct strbuf *b){
    int temp_len = a->len;
    int temp_alloc = a->alloc;
    char *temp_buf = a->buf;
    a->len = b->len;
    a->alloc = b->alloc;
    a->buf = b->buf;
    b->len = temp_len;
    b->alloc = temp_alloc;
    b->buf = temp_buf;
}
char *strbuf_detach(struct strbuf *sb, size_t *sz){
    *sz=sb->alloc;
    char*str=sb->buf;
    return str;
}
int strbuf_cmp(const struct strbuf *first, const struct strbuf *second){
    int n=(first->len>second->len?second->len:first->len);
    for(int i=0;i<n;i++){
        if(*(first->buf+i)-*(second->buf+i)!=0){
            return *(first->buf+i)-*(second->buf+i);
        }
    }
    return 0;
}
void strbuf_reset(struct strbuf *sb){
    sb->len=0;
    sb->buf[0]='\0';
}
//Part 2B
void strbuf_grow(struct strbuf *sb, size_t extra){
    while(sb->alloc<sb->len+extra){
        char*temp=(char*)malloc(sb->alloc*2);
        memcpy(temp,sb->buf,sb->len);
        free(sb->buf);
        sb->buf=temp;
        sb->alloc*=2;
    }
}
void strbuf_add(struct strbuf *sb, const void *data, size_t len){
    strbuf_grow(sb,len);
    memcpy(sb->buf+sb->len,(char*)data,len);
    sb->len+=len;
    sb->buf[sb->len] = '\0';
}
void strbuf_addch(struct strbuf *sb, int c){
    strbuf_grow(sb,1);
    char temp[2];
    temp[0]=(char)c;
    temp[1]='\0';
    strncat(sb->buf,temp,1);
    sb->len+=1;
}
void strbuf_addstr(struct strbuf *sb, const char *s){
    int n=strlen(s);
    strbuf_grow(sb,n);
    memcpy(sb->buf+sb->len,s,n);
    sb->len+=n;
    sb->buf[sb->len]='\0';
}
void strbuf_addbuf(struct strbuf *sb, const struct strbuf *sb2){
    strbuf_grow(sb,sb2->len);
    memcpy(sb->buf+sb->len,sb2->buf,sb2->len);
    sb->len+=sb2->len;
    sb->buf[sb->len]='\0';
}
void strbuf_setlen(struct strbuf *sb, size_t len){
    if(sb->alloc<len){
    strbuf_grow(sb,len-sb->alloc);
    }
    sb->len=len;
    sb->buf[len]='\0';
}
size_t strbuf_avail(const struct strbuf *sb){
    return sb->alloc-sb->len-1;
}
void strbuf_insert(struct strbuf *sb, size_t pos, const void *data, size_t len){
    strbuf_grow(sb,len);
    memmove(sb->buf+pos+len,sb->buf+pos,len-pos);
    memcpy(sb->buf,(char*)data,len);
    sb->len+=len;
    sb->buf[sb->len]='\0';
}
//Part 2C
void strbuf_ltrim(struct strbuf *sb){
    int start=0;
    while(start<sb->len&&isspace((unsigned char)sb->buf[start])){
        start++;
    }
    if(start>0){
        int len=sb->len-start;
        memmove(sb->buf,sb->buf+start,len);
        sb->buf[len]='\0';
        sb->len=len;
    }
}
void strbuf_rtrim(struct strbuf *sb){
    int end=sb->len-1;
    while(end>=0&&isspace((unsigned char)sb->buf[end])){
        end--;
    }
    if(end!=sb->len-1){
        sb->len=end+1;
        sb->buf[end+1]='\0';
    }
}
void strbuf_remove(struct strbuf *sb, size_t pos, size_t len){
    if(pos+len>sb->len){
        return ;
    }
    memmove(sb->buf+pos,sb->buf+pos+len,sb->len-pos-len);
    sb->len-=len;
    sb->buf[sb->len]='\0';
}
//Part 2D
ssize_t strbuf_read(struct strbuf *sb, int fd, size_t hint){
    strbuf_grow(sb,(hint?hint:8192));
    int len=read(fd,sb->buf,(hint?hint:8192));
    sb->len=len;
    sb->buf[sb->len]='\0';
    return len;
}
int strbuf_getline(struct strbuf *sb, FILE *fp){
    char c;
    c=fgetc(fp);
    while(c!='\n'&&c!=EOF){
        strbuf_addch(sb,c);
        c=fgetc(fp);
    }
    return sb->len;
}
//CHALLENGE
struct strbuf** strbuf_split_buf(const char* str, size_t len, int terminator, int max){
    struct strbuf **result=(struct strbuf**)malloc((max+1)*sizeof(struct strbuf*));
    int start=0;
    int count=0;
    while(count<max){
        const char*temp=strchr(str+start,terminator);
        if(temp==NULL){
            int len1=len-start;
            result[count]=(struct strbuf*)malloc(sizeof(struct strbuf));
            result[count]->buf=(char*)malloc(len1+1);
            strncpy(result[count]->buf,temp,len1);
            result[count]->alloc=len1+1;
            result[count]->len=len1;
            result[count]->buf[len1]='\0';
            result[count+1]=NULL;
            break;
        }
        else{
            int len2=temp-(str+start);
            result[count]=(struct strbuf*)malloc(sizeof(struct strbuf));
            result[count]->buf=(char*)malloc(len2+1);
            strncpy(result[count]->buf,temp,len2);
            result[count]->alloc=len2+1;
            result[count]->len=len2;
            result[count]->buf[len2]='\0';
            start=temp-str+1;
        }
        if(count==max){
            result[count]=NULL;
        }
        count++;
    }
    return result;
}
bool strbuf_begin_judge(char* target_str, const char* str, int strlen){
    return str==NULL||!strncmp(target_str, str, strlen);
}
char* strbuf_get_mid_buf(char* target_buf, int begin, int end, int len){
    if(begin<0||len<=end||begin>end){
        return NULL;
    }
    char*result=(char*)malloc(end-begin+1);
    memcpy(result,target_buf+begin,end-begin);
    result[end-begin]='\0';
    return result;
}