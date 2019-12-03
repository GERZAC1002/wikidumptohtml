#include <stdio.h>
#include <stdlib.h>

char loesch[100][100] = {
	"<mediawiki ",
	"<id>",
	"<username>",
	"</username>",
	"<mediawiki>",
	"<ip>",
	"<timestamp>",
	"<contributor>",
	"</contributor>",
	"<revision>",
	"</revision>",
	"<ns>",
	"<format>",
	"<model>",
	"<sha1>",
	"<namespace ",
	"<namespaces>",
	"</namespaces>",
	"<case>",
	"<comment>",
	"<minor />",
	"<dbname>",
	"<base>",
	"<sitename>",
	"<generator>",
	"<parentid>",
	"<siteinfo>",
	"</siteinfo>"
};

char ersetz[100][100] = {
	"[[",
	"]]",
	"{{",
	"}}",
	"''",
	"<small>",
	"</small>"
};

int strlength(char *string){
	int i=0;
	while(string[i]){
		i++;
	}
	return i;
}

int strcomp(char *string1,char *string2){
	int result = -1;
	for(int i=0;(i<strlength(string1)) && (result == -1);i++){
		if(string1[i] == string2[0]){
			for(int j=0;j<strlength(string2);j++){
				if(string1[i+j]!=string2[j]){
					result = -1;
					break;
				}else{
					result = i;
				}
			}
		}
	}
	return result;	
}

int strdel(char *string1){
	int res=-1;
	for(int i=0;i<100;i++){
		if(strcomp(string1,loesch[i])!=-1){
			res = 0;
		}
	}
	return res;
}

int strersetz(char *string1){
	int res=-1;
	for(int i=0;i<100;i++){
		int pos = strcomp(string1,ersetz[i]);
		if(pos != -1){
			for(int i=0;i<strlength(ersetz[i]);i++){
				string1[pos+i]=' ';
			}
			res=0;
		}
	}
	return res;
}

int main(int argc,char *argv[]){
	int c;
	char zeile[10000];
	char zeilezeigen = 1;
	char titel[100];
	char seite[106];
	int index = 0;
	int seitenzahl = 0;
	FILE *datei;
	FILE *dump;
	if((dump = fopen(argv[1],"r")) == NULL){
		printf("Konnte Eingabedatei %s nicht lesen!\n",argv[1]);
		printf("Programmaufruf: wikidumptohtml <dumpfile>\n");
		return 0;
	}
	while((c = getc(dump))!=EOF){
		if(c == '\n'){
			//if(strcomp(zeile,suche)!=-1){
			if(strdel(zeile)!=-1){
			}else{
				while(strersetz(zeile)!=-1){}
				if(strcomp(zeile,"<page>")!=-1){
					zeilezeigen = 0;
					seitenzahl = seitenzahl + 1;
				}
				if(strcomp(zeile,"<title>")!=-1){
					for(int i = 0;i < strlength(zeile)-19;i++){
						titel[i] = zeile[strcomp(zeile,"<title>")+7+i];
						titel[i+1] = '\0';
					}
					for(int i = 0;titel[i]!='\0';i++){
						seite[i]=titel[i];
					}
					int seitel = strlength(seite);
					for(int i = 0; i < seitel;i++){
						if(seite[i] == '/'){
							seite[i] = '_';
						}
					}
					seite[seitel+0] = '.';
					seite[seitel+1] = 'h';
					seite[seitel+2] = 't';
					seite[seitel+3] = 'm';
					seite[seitel+4] = 'l';
					seite[seitel+5] = '\0';
					printf("%d.Seite:%s\n",seitenzahl,seite);
					if((datei = fopen(seite,"w"))==NULL){
						printf("Konnte Datei nicht öffnen!\n");
						return 1;
					}
					for(int i = 0;seite[i] != '\n';i++){
						seite[i]='\0';
					}
					for(int i = 0;titel[i] != '\0';i++){
						titel[i]='\0';
					}
					fprintf(datei,"<!DOCTYPE html>\n<html>\n\t<head>\n\t\t<title>%s</title>\n\t</head>\n\t<body>\n\t<h1>%s</h1>\n\t<article>",titel,titel);
					zeilezeigen = 0;
				}
				if(strcomp(zeile,"</page>")!=-1){
					fprintf(datei,"</article>\n\t</body>\n</html>");
					fclose(datei);
					zeilezeigen = 0;
				}
				if((strcomp(zeile,"== ")!=1)&&(strcomp(zeile," ==")!=-1)){
					fprintf(datei,"</article>\n<h2>%s</h2>\n<article>\n",zeile);
					zeilezeigen = 0;
				}
				if((strcomp(zeile,"=== ")!=1)&&(strcomp(zeile," ===")!=-1)){
					fprintf(datei,"</article>\n<h3>%s</h3>\n<article>\n",zeile);
					zeilezeigen = 0;
				}
				if(zeilezeigen == 1){
					fprintf(datei,"%s<br>\n",zeile);
				}
				zeilezeigen = 1;
			}
			for(int i=0;zeile[i]!='\0';i++){
				zeile[i]='\0';
			}
			index = 0;
		}else{
			zeile[index] = c;
			index = index + 1;
		}
	}
	fclose(datei);
	return 0;
}
