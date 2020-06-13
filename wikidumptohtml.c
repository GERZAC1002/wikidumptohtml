#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char loesch[50][100] = {
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

char ersetz[50][10] = {
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
	while(string[i]!= '\0'){
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
	for(int i=0;i<50;i++){
		if(strcomp(string1,loesch[i])!=-1){
			res = 0;
		}
	}
	return res;
}

int strersetz(char *string1){
	int res=-1;
	for(int i=0;i<50;i++){
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

int clrarray(char *string){
	for(int i = 0;string[i] != '\0';i++){
		string[i] = '\0';
	}
	return 0;
}

int main(int argc,char *argv[]){
	long c;
	long lange = 1000;
	char *zeile = malloc(lange);
	register char zeilezeigen = 1;
	register char seitezeigen = 1;
	char titel[1000];
	char seite[1006];
	char *page = "  <page>";
	register long index = 0;
	register long seitenzahl = 0;
	register long maxzeile = 0;
	FILE *datei;
	FILE *dump;
	if((dump = fopen(argv[1],"r")) == NULL){
		printf("Konnte Eingabedatei %s nicht lesen!\n",argv[1]);
		printf("Programmaufruf: wikidumptohtml <dumpfile>\n");
		return 0;
	}
	while((c = getc(dump))!=EOF){
		if(c == '\n'){
			if(strdel(zeile)!=-1){
			}else{
				char p = 1;
				for(int i = 0;i < strlength(page);i++){
					if(zeile[i] != page[i]){
						p = 0;
						break;
					}
				}
				if(p == 1){
					zeilezeigen = 0;
					seitenzahl = seitenzahl + 1;
				}
				if(strcomp(zeile,"<title>")!=-1){
					seitezeigen = 1;
					for(int i = 0;i < strlength(zeile)-19;i++){
						titel[i] = zeile[strcomp(zeile,"<title>")+7+i];
						titel[i+1] = '\0';
					}
					for(int i = 0;titel[i]!='\0';i++){
						seite[i]=titel[i];
					}
					int seitel = strlength(seite);
					for(int i = 0; i < strlength(seite);i++){
						if(seite[i] == '/'){
							seite[i] = '_';
						}
						if(seite[i] == ' '){
							seite[i] = '_';
						}
						if(seite[i] == '('){
							seite[i] = '_';
						}
						if(seite[i] == ')'){
							seite [i] = '_';
						}
					}
					seite[seitel+0] = '.';
					seite[seitel+1] = 'h';
					seite[seitel+2] = 't';
					seite[seitel+3] = 'm';
					seite[seitel+4] = 'l';
					seite[seitel+5] = '\0';
					printf("%ld.Seite:%s\n",seitenzahl,seite);
					if((datei = fopen(seite,"r")) != NULL){
						datei = NULL;
						printf("Seite %s schon erstellt!\n",seite);
						seitezeigen = 0;
					}else{
						if((datei = fopen(seite,"w")) == NULL){
							printf("Konnte Datei %s nicht öffnen!\n",seite);
							return 1;
						}
						fprintf(datei,"<!DOCTYPE html>\n<html>\n\t<head>\n\t\t<title>%s</title>\n\t\t<meta charset=\"UTF-8\" />\n\t</head>\n\t<body>\n\t<h1>%s</h1>\n\t<article>",titel,titel);
					}
					zeilezeigen = 0;
					clrarray(seite);
					clrarray(titel);
				}
				if(datei != NULL && seitezeigen != 0){
					if(strstr(zeile,"  </page>")!=NULL){
						fprintf(datei,"</article>\n\t</body>\n</html>");
						fclose(datei);
						zeilezeigen = 0;
					}else{
						while(strersetz(zeile)!=-1){}
					}
					if((strcomp(zeile,"== ")!=1)&&(strcomp(zeile," ==")!=-1)){
						fprintf(datei,"</article>\n<h2>%s</h2>\n<article>\n",zeile);
						zeilezeigen = 0;
					}else if((strcomp(zeile,"=== ")!=1)&&(strcomp(zeile," ===")!=-1)){
						fprintf(datei,"</article>\n<h3>%s</h3>\n<article>\n",zeile);
						zeilezeigen = 0;
					}
					if(zeilezeigen == 1){
						fprintf(datei,"%s<br>\n",zeile);
					}
					zeilezeigen = 1;
				}
			}
			if(maxzeile < strlength(zeile) && maxzeile > 0){
				printf("Max. Zeilenlänge: %ld\n",maxzeile);
			}
			for(int i=0;zeile[i]!='\0';i++){
				zeile[i]='\0';
			}
			index = 0;
		}else{
			if(index < lange){
				zeile[index] = c;
				index = index + 1;
			}else{
				char *tmp = calloc(lange,1);
				for(int i=0; i < lange;i++){
					tmp[i] = zeile[i];
				}
				free(zeile);
				zeile = (char *)calloc(2*lange,1);
				if(zeile == NULL){
					printf("Konnte Speicher nich reservieren!\n");
				}
				for(int i=0; i < lange;i++){
					zeile[i] = tmp[i];
				}
				free(tmp);
				lange = lange*2;
				printf("Neue Zeilenlänge: %ld\n",lange);
				zeile[index] = c;
				index = index + 1;
			}
		}
	}
	free(zeile);
	fclose(dump);
	return 0;
}
