#include <stdio.h>
#include <windows.h>

void main()
{
            HANDLE hFile;
            OVERLAPPED over;
            DWORD dByte,dwError;
            int iByteToRead=1;
            BOOL bResult;
            int iWord=0,tek=1,pred=0,iBreak=0,iBegin=1,iEnd=1;      //новое слово - tek=1,pted=0;
           
                       
            over.Offset=0;
            over.OffsetHigh=0;
           
            char cBuffer[1];
            // —оздаЄм событие дл€ контрол€ за асинхронным чтением
            over.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
            if(over.hEvent == NULL)
            {
                        printf("\nError create event!!!");
                        exit(0);
                        // ќшибка создани€ событи€ Е
            }
           
            hFile=CreateFile("111.txt",GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL|FILE_FLAG_OVERLAPPED,NULL);
            if(hFile==INVALID_HANDLE_VALUE)
            {
                        printf("\nError opening file!!!");
                        exit(0);
            }
            while(1)
            {
                        bResult = ReadFile(hFile,cBuffer,iByteToRead,&dByte,&over);
                        // если возникает проблема или асинхронна€ операци€
                      
                       
                        if (!bResult)
                        {
                                   // решаем что делать с кодом ошибки
                                   switch (dwError = GetLastError())
                                   {
                                   case ERROR_HANDLE_EOF:
                                               {
                                                           // мы достигли конца файла
                                                           // в течение вызова к ReadFile
                                                           iBreak=1;
                                                           break;
                                               }
                                              
                                   case ERROR_IO_PENDING:
                                               {
                                                           // асинхронный ввод-вывод все еще происходит
                                                          
                                                           // сделаем кое-что пока он идет
                                                         
                                                           over.Offset++;
                                                          
                                                           pred=tek;
                                                           if(cBuffer[0]==' '|| cBuffer[0]=='\x0D' || cBuffer[0]=='\n')tek=0;
                                                           else tek=1;
                                                           if(tek==1 && pred==0)iWord++;
                                                           // проверим результат работы асинхронного чтени€
                                                           bResult = GetOverlappedResult(hFile, &over,         &dByte, FALSE) ;
                                                          
                                                           // если возникла проблема ...
                                                           if (!bResult)
                                                           {
                                                                       // решаем что делать с кодом ошибки
                                                                       switch (dwError = GetLastError())
                                                                       {
                                                                       case ERROR_HANDLE_EOF:
                                                                                  {
                                                                                              // мы достигли конца файла
                                                                                              // в ходе асинхронной
                                                                                              // операции
                                                                                              iBreak=1;
                                                                                              break;
                                                                                  }
                                                                       default:
                                                                                  {
                                                                                              printf("\nError work to file!!!");
                                                                                              iBreak=1;
                                                                                              break;// решаем что делать с другими случа€ми ошибок
                                                                                  }
                                                                       }// конец процедуры switch (dwError = GetLastError())
                                                           }
                                                           break;
                                               } // конец процедуры case
                                   default:
                                               {
                                                           printf("\nError work to file!!!");
                                                           iBreak=1;
                                                           break;  // решаем что делать с другими случа€ми ошибок
                                               }
                                              
                                   } // конец процедуры switch (dwError = GetLastError())
                        } // конец процедуры if
                        if(iBreak==1)break;
                        over.Offset++;
                        pred=tek;
                        if(cBuffer[0]==' ' || cBuffer[0]=='\x0D' || cBuffer[0]=='\n')tek=0;
                        else tek=1;
                        if(tek==1 && pred==0)iWord++;
                       
                       
            }
            printf("\nIn text %d words!!!", iWord);
            getchar();
}