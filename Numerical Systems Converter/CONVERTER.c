#include "StdTypes.h"
#include "Utils.h"
#include "DIO_Int.h"
#include "LCD_Init.h"
#include "KPD_Int.h"

#include "CONVERTER.h"

#define  F_CPU  8000000UL
#include <util/delay.h>

#define DECIMALLINE  Line2
#define BINARYLINE   Line3
#define HEXALINE     Line4



typedef enum
{
	DECIMAL='1',
	BINARY='2',
	HEXA ='3',
	
}CONV_Option_t;



static u8 str[]=" Convert From : ";
static u8 str1[]=" 1.Decimal.. ";
static u8 str2[]=" 2.Binary.. ";
static u8 str3[]=" 3.Hexadecimal.. ";

static u8 NumStr[20]={0};

static u16 DecimalNum=0;
static u8 Binarystr[20]={0};
static u8 Hexastr[10]={0};

static u8 index=0;

CONV_Option_t key=NO_KEY;
CONV_Option_t ConvertFrom;
CONV_Option_t LastOption;


static Flag_t flag={1,0,0,0,0,0,0,0};
/*
f1 >>> convert opption frame
f2 >>> scan operation
f3 >>> Scan Number
f4  >>>> finshed opp

*/


static void CONVERTER_OptionPrint(void)
{
	LCD_SetCursor(LCD1,Line1,Cell_1);
	LCD_WriteString(LCD1,str);

	
	LCD_SetCursor(LCD1,DECIMALLINE,Cell_1);
	LCD_WriteString(LCD1,str1);
	
	LCD_SetCursor(LCD1,BINARYLINE,Cell_1);
	LCD_WriteString(LCD1,str2);
	
	LCD_SetCursor(LCD1,HEXALINE,Cell_1);
	LCD_WriteString(LCD1,str3);
}


void CONVERTER_Init(void)
{
	//	CONVERTER_OptionPrint();
}


static void CONVERTER_GetOption(void)
{
	u8 str[]="Enter Decimal Number";
	u8 str1[]="Enter Binary Number";
	u8 str2[]="Enter Hexa Number";
	
	if (flag.f8==0)
	{
		key=KEYPAD_GetKey();
		
		LastOption=key;
		if ((key!=NO_KEY && (key>='1' && key<='3'))||(flag.f8==1))
		{
			
			flag.f2=0;
			flag.f3=1;
			
			switch (LastOption)
			{
				case DECIMAL :
				ConvertFrom=DECIMAL;
				LCD_ClearLineDisplay(LCD1,BINARYLINE);
				LCD_ClearLineDisplay(LCD1,HEXALINE);
				_delay_ms(100);
				LCD_ClearDisplay(LCD1);
				
				LCD_SetCursor(LCD1,Line1,Cell_1);
				LCD_WriteString(LCD1,str);
				
				LCD_SetCursor(LCD1,Line2,Cell_2);
				
				break;
				
				case BINARY :
				ConvertFrom=BINARY;
				LCD_ClearLineDisplay(LCD1,DECIMALLINE);
				LCD_ClearLineDisplay(LCD1,HEXALINE);
				_delay_ms(100);
				LCD_ClearDisplay(LCD1);
				
				LCD_SetCursor(LCD1,Line1,Cell_1);
				LCD_WriteString(LCD1,str1);
				
				LCD_SetCursor(LCD1,Line2,Cell_2);
				LCD_WriteChar2(LCD1,'0');
				LCD_WriteChar2(LCD1,'B');
				break;
				
				case HEXA :
				ConvertFrom=HEXA;
				LCD_ClearLineDisplay(LCD1,DECIMALLINE);
				LCD_ClearLineDisplay(LCD1,BINARY);
				_delay_ms(100);
				LCD_ClearDisplay(LCD1);
				
				LCD_SetCursor(LCD1,Line1,Cell_1);
				LCD_WriteString(LCD1,str2);
				
				LCD_SetCursor(LCD1,Line2,Cell_2);
				LCD_WriteChar2(LCD1,'0');
				LCD_WriteChar2(LCD1,'X');
				break;
				
			}
			
		}
		
	}
	else
	{
		
		flag.f2=0;
		flag.f3=1;
		
		switch (LastOption)
		{
			case DECIMAL :
			ConvertFrom=DECIMAL;
			
			LCD_SetCursor(LCD1,Line1,Cell_1);
			LCD_WriteString(LCD1,str);
			
			LCD_SetCursor(LCD1,Line2,Cell_2);
			
			break;
			
			case BINARY :
			ConvertFrom=BINARY;
		
			LCD_SetCursor(LCD1,Line1,Cell_1);
			LCD_WriteString(LCD1,str1);
			
			LCD_SetCursor(LCD1,Line2,Cell_2);
			LCD_WriteChar2(LCD1,'0');
			LCD_WriteChar2(LCD1,'B');
			break;
			
			case HEXA :
			ConvertFrom=HEXA;
		
			LCD_SetCursor(LCD1,Line1,Cell_1);
			LCD_WriteString(LCD1,str2);
			
			LCD_SetCursor(LCD1,Line2,Cell_2);
			LCD_WriteChar2(LCD1,'0');
			LCD_WriteChar2(LCD1,'X');
			break;
			
		}
		
	}
	
	
	
}

/*
static u16 String_To_Integer(u8 *str)
{
	u8 i=0;
	u8 flag=0;
	u16 res=0;
	u16 Num=0;

	while (str[i]==' ')
	{
		i++;
	}
	if (str[i]=='-')
	{
		flag=1;
		i++;
	}
	for (;str[i];i++)
	{
		if (str[i]>='0' && str[i]<='9')
		{
			res=str[i]-'0';
			Num=(Num*10)+res;
		}
	}
	if (flag==1)
	{
		Num=Num*-1;
	}

	return Num;
}
*/


static void CONVERTER_ScanNumber(void)
{
	key=KEYPAD_GetKey();
	
	if (key!=NO_KEY)
	{
		if (key>='0' && key<='9')
		{
			LCD_WriteChar2(LCD1,key);
			NumStr[index]=key;
			index++;
			
		}
		
		else if (key>='A'&& key<='E')
		{
			LCD_WriteChar2(LCD1,key);
			NumStr[index]=key;
			index++;
		}

		else if (key=='c')
		{
			NumStr[index]='\0';
			index=0;
			flag.f3=0;
			flag.f4=1;
			
		}
		
	}
	
	if (DIO_Button_CheckStatus(FCHARPIN)==PRESSED)
	{
		LCD_WriteChar2(LCD1,'F');
		NumStr[index]='F';
		index++;
		while(DIO_Button_CheckStatus(FCHARPIN)==PRESSED);
		
	}
	

}


static void Decimal_TO_Hexa_str(u16 num , u8*hexastr)
{
	u8 HB1=(num&(u16)0x00f0)>>4;
	u8 LB1=num&(u16)0x000f;
	u8 HB2=(num&(u16)0xf000)>>12;
	u8 LB2=(num&(u16)0x0f00)>>8;
	u8 i=0;
	hexastr[i]='0';
	i++;
	hexastr[i]='X';
	i++;
	
	if (HB2<10 /*&& LB2!=0*/)
	{
		hexastr[i]=(HB2+'0');
		i++;
	}
	else if (HB2>=10)
	{
		hexastr[i]=(HB2+'A'-10);
		i++;
	}
	
	if (LB2<10 /*&& HB1!=0*/)
	{
		hexastr[i]=(LB2+'0');
		i++;
	}
	else if (LB2>=10)
	{
		hexastr[i]=(LB2+'A'-10);
		i++;
	}
	
	if (HB1<10/* && LB1 !=0*/)
	{
		hexastr[i]=(HB1+'0');
		i++;
	}
	else if (HB1>=10 )
	{
		hexastr[i]=(HB1+'A'-10);
		i++;
	}
	
	
	if (LB1<10)
	{
		hexastr[i]=(LB1+'0');
		i++;
	}
	else
	{
		hexastr[i]=(LB1+'A'-10);
		i++;
	}
	
	hexastr[i]='\0';
	
	i=0;
	/*
	while(hexastr[i]==0 &&hexastr[i+1]==0 )
	{
	for (j=0;hexastr[j];j++ )
	{
	hexastr[j]=hexastr[j+1];
	}
	i++;
	}
	
	*/

}



static void Decimal_TO_Binary_str(u16 num , u8*binarystr)
{
	s8 i=0,j=0;
	u8 f1=0;
	
	binarystr[j]='0';
	j++;
	binarystr[j]='B';
	j++;
	
	for (i=16;i>=0;i--)
	{
		if (READ_BIT(num,i)==1 ||READ_BIT(num,(i-1))==1)
		{
			f1=1;
		}
		
		if (f1==1)
		{
			if (READ_BIT(num,i)==1)
			{
				binarystr[j]='1';
				j++;
			}
			else
			{
				binarystr[j]='0';
				j++;
			}
		}
		
	}
	binarystr[j]='\0';


}



static void Binary_TO_Decimal(u8* binarystr,u16 *decimalnum)
{
	s8 i=0,j=0;
	u8 mystr[20];
	u8 size=0;
	if (binarystr[i]=='0'&&binarystr[i+1]=='B')
	{
		i=2;
	}
	else
	{
		i=0;
	}
	
	for (i;binarystr[i];i++)
	{
		mystr[size]=binarystr[i];
		size++;
	}
	
	for (i=size-1 ; i>=0 ; i--)
	{
		if (mystr[i]=='1')
		{
			SET_BIT(*decimalnum,j);
			j++;
		}
		else
		{
			CLR_BIT(*decimalnum,j);
			j++;
		}
	}
	
	while(j<=16)
	{
		CLR_BIT(*decimalnum,j);
		j++;
	}
	
}


static void Binary_TO_Hexa(u8* binarystr,u8 *hexastr)
{
	u16 decimalnum=0;
	
	Binary_TO_Decimal(binarystr,&decimalnum);
	
	Decimal_TO_Hexa_str(decimalnum,hexastr);
	
}


static void Hexa_TO_Decimal(u8* hexastr,u16 *decimalnum)
{
	u16 num=0;
	u16 V4B=0;
	u8 size=0,j=0;
	s8 i=0;
	u8 mystr[10];
	
	if (hexastr[i]=='0' && hexastr[i+1]=='X')
	{
		i=2;
	}
	else
	{
		i=0;
	}

	
	for (i; hexastr[i];i++)
	{
		mystr[size]=hexastr[i];
		size++;
	}
	mystr[size]='\0';
	
	
	for (i=size-1 ; i>=0 ; i--)
	{
		
		
		if (mystr[i]<'A')
		{
			V4B=mystr[i]-'0';
			
		}
		else if (mystr[i]>='A')
		{
			V4B=mystr[i]-'A'+10;
		}
		
		
		num=num|((V4B)<<(j*4));
		j++;
	}
	
	*decimalnum=num;
	
	
}


static void Hexa_TO_Binary(u8* hexastr,u8* binarystr)
{
	u16 decimalnum=0;
	
	Hexa_TO_Decimal(hexastr,&decimalnum);
	
	Decimal_TO_Binary_str(decimalnum,binarystr);
	
}



static void CONVERTER_Convert(void)
{

	switch (ConvertFrom)
	{
		case DECIMAL :
		
		DecimalNum=String_To_Integer(NumStr);
		
		Decimal_TO_Binary_str(DecimalNum,Binarystr);
		Decimal_TO_Hexa_str(DecimalNum,Hexastr);
		
		LCD_SetCursor(LCD1,Line3,Cell_2);
		LCD_WriteString(LCD1,Binarystr);
		
		LCD_SetCursor(LCD1,Line4,Cell_2);
		LCD_WriteString(LCD1,Hexastr);
		
		break;
		
		
		case BINARY :
		
		Binary_TO_Decimal(NumStr,&DecimalNum);
		Binary_TO_Hexa(NumStr,Hexastr);
		
		LCD_SetCursor(LCD1,Line3,Cell_2);
		LCD_WriteNumber(LCD1,(f64)DecimalNum);
		
		LCD_SetCursor(LCD1,Line4,Cell_2);
		LCD_WriteString(LCD1,Hexastr);
		
		break;
		
		
		
		case HEXA :
		
		Hexa_TO_Decimal(NumStr,&DecimalNum);
		Hexa_TO_Binary(NumStr,Binarystr);
		
		LCD_SetCursor(LCD1,Line3,Cell_2);
		LCD_WriteNumber(LCD1,(f64)DecimalNum);
		
		LCD_SetCursor(LCD1,Line4,Cell_2);
		LCD_WriteString(LCD1,Binarystr);
		
		break;
	}
	
}

/*
void CONVERTER_Runable2(void)
{

	if (flag.f1)
	{
		CONVERTER_OptionPrint();
		flag.f1=0;
		flag.f2=1;
	}
	
	if (flag.f2)
	{
		CONVERTER_GetOption();
	}
	
	if (flag.f3)
	{
		CONVERTER_ScanNumber();
		
	}
	else
	{
		
		if (flag.f4)
		{
			CONVERTER_Convert();
			flag.f4=0;
			flag.f5=1;
		}
		
		if (flag.f5)
		{
			key=KEYPAD_GetKey();
			
			if (key!=NO_KEY)
			{
				LCD_ClearDisplay(LCD1);
				
				flag.f5=0;
				flag.f1=1;
			}
		}
		
	}
	
	
}

*/



void CONVERTER_Runable(void)
{
	
		
		if (flag.f1)
		{
			if (flag.f8==0)
			{
					CONVERTER_OptionPrint();
					flag.f1=0;
					flag.f2=1;
			}
			else
			{
				flag.f1=0;
				flag.f2=1;
			}
		
		}
		
		if (flag.f2)
		{
		//	if (flag.f8==0)
			CONVERTER_GetOption();
		}
		
		if (flag.f3)
		{
			CONVERTER_ScanNumber();
			
		}
		else
		{
			
			
			if (flag.f4)
			{
				CONVERTER_Convert();
				flag.f4=0;
				flag.f5=1;
			}
			
			if (flag.f5)
			{
				key=KEYPAD_GetKey();
				
				if (key!=NO_KEY )
				{
					if (key=='c')
					{
						LCD_ClearDisplay(LCD1);
						flag.f5=0;
						flag.f1=1;
						flag.f8=0;
					}
					else
					{
						LCD_ClearDisplay(LCD1);
						flag.f5=0;
						flag.f1=1;
						flag.f8=1;
						
						
					}
					
				}
				
			}
			
		}
		
		
	}
	
	
	













