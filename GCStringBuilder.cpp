#include "stdafx.h"
#include "GCStringBuilder.h"
#include "GCStrings.h"
#include "GCDisplaySettings.h"

GCStringBuilder::GCStringBuilder(void)
{
	Target = NULL;
}


GCStringBuilder::~GCStringBuilder(void)
{
}

void GCStringBuilder::AppendString(const char * text)
{
	(*Target) += text;
}

void GCStringBuilder::AppendBoldString(const char * text)
{
	if (Format == SBTF_TEXT)
	{
		(*Target) += text;
	}
	else if (Format == SBTF_RTF)
	{
		(*Target) += "{\\b ";
		(*Target) += text;
		(*Target) += "}";
	}
}

void GCStringBuilder::AppendLine(const char * text)
{
	switch(Format)
	{
	case SBTF_TEXT:
		(*Target) += text;
		(*Target) += "\r\n";
		break;
	case SBTF_RTF:
		(*Target) += "\r\n\\par ";
		(*Target) += text;
		break;
	case SBTF_HTML:
		(*Target) += "<p>";
		(*Target) += text;
		break;
	}

}


void GCStringBuilder::initWithString(TString * str)
{
	Target = str;
}


void GCStringBuilder::AppendHeader1(const char * text)
{
	if (Format == SBTF_TEXT)
	{
	}
	else if (Format == SBTF_RTF)
	{
		(*Target) += "\\par\\pard{\\f2\\fs";
		(*Target) += fontSizeH1;
		(*Target) += " ";
		(*Target) += text;
		(*Target) += "}";
	}
}

void GCStringBuilder::AppendHeader2(const char * text)
{
	if (Format == SBTF_TEXT)
	{
	}
	else if (Format == SBTF_RTF)
	{
		(*Target) += "\\par\\pard{\\f2\\fs";
		(*Target) += fontSizeH2;
		(*Target) += " ";
		(*Target) += text;
		(*Target) += "}";
	}
}



void GCStringBuilder::AppendDocumentHeader(void)
{
	if (Format == SBTF_TEXT)
	{
	}
	else if (Format == SBTF_RTF)
	{
		GCStringBuilder::AppendRtfHeader(Target);
		(*Target) += "\\par\\pard\\f2\\fs";
		(*Target) += fontSizeText;
		(*Target) += "\r\n";
	}
}

void GCStringBuilder::AppendDocumentTail(void)
{
	if (Format == SBTF_TEXT)
	{
	}
	else if (Format == SBTF_RTF)
	{
		(*Target) += "}";
	}
}

void GCStringBuilder::AppendSeparatorWithWidth(int width)
{
	static const char * szSeparator = "----------------";
	int len = strlen(szSeparator);

	while(width > len)
	{
		AppendString(szSeparator);
		width -= len;
	}

	AppendString(szSeparator + len - width);
}

void GCStringBuilder::AppendNote(void)
{
	AppendLine();
	AppendLine();
	AppendSeparatorWithWidth(80);
	AppendLine();
	AppendBoldString("Notes:");
	AppendLine();
	AppendLine();
	AppendLine("DST - Time is in \'Daylight Saving Time\'");
	AppendLine("LT  - Time is in \'Local Time\'\r\n");

	if (GCDisplaySettings::getValue(9) > 0 || GCDisplaySettings::getValue(10) > 0 || GCDisplaySettings::getValue(11) > 0 || GCDisplaySettings::getValue(12) > 0)
	{
		AppendLine("(*) - value at the moment of sunrise");
	}

	// last line
	AppendLine();
	AppendString("Generated by ");
	AppendString(GCStrings::getString(131));
}

void GCStringBuilder::AppendRtfHeader(TString * m_text)
{
	*m_text = "{\\rtf1\\ansi\\ansicpg1252\\deff2\\deflang1033{\\fonttbl{\\f0\\fswiss\\fcharset0 Lucida Console;}"
			"{\\f1\\fswiss\\fcharset0 Arial;}{\\f2\\froman\\fprq2\\fcharset0 Book Antiqua;}}"
			"{\\colortbl ;";
	AppendRtfColorTable(m_text);
	*m_text +=  "}{\\*\\generator GCAL;}\\viewkind4\\uc1\\pard\\f0\\fs20 ";
}

void GCStringBuilder::AppendRtfColorTable(TString * str)
{
	*str += "\\red0\\green0\\blue0;" //0
		"\\red16\\green16\\blue16;" //1
		"\\red32\\green32\\blue32;" //2
		"\\red48\\green48\\blue48;" //3
		"\\red64\\green64\\blue64;" //4
		"\\red80\\green80\\blue80;" //5
		"\\red96\\green96\\blue96;" //6
		"\\red112\\green112\\blue112;" //7
		"\\red128\\green128\\blue128;" // 8 
		"\\red143\\green143\\blue143;" // 9 
		"\\red159\\green159\\blue159;" // 10 
		"\\red175\\green175\\blue175;" // 11 
		"\\red191\\green191\\blue191;" // 12 
		"\\red207\\green207\\blue207;" // 13 
		"\\red223\\green223\\blue223;" // 14 
		"\\red239\\green239\\blue239;" // 15 
		"\\red255\\green255\\blue255;" // 16 
		"\\red0\\green0\\blue0;" //17
		"\\red31\\green0\\blue0;" //18
		"\\red63\\green0\\blue0;" //19
		"\\red95\\green0\\blue0;" //20
		"\\red127\\green0\\blue0;" //21
		"\\red159\\green0\\blue0;" //22
		"\\red191\\green0\\blue0;" //23
		"\\red223\\green0\\blue0;" //24
		"\\red255\\green0\\blue0;" // 25 
		"\\red255\\green31\\blue31;" // 26 
		"\\red255\\green63\\blue63;" // 27 
		"\\red255\\green95\\blue95;" // 28 
		"\\red255\\green127\\blue127;" // 29 
		"\\red255\\green159\\blue159;" // 30 
		"\\red255\\green191\\blue191;" // 31 
		"\\red255\\green223\\blue223;" // 32 
		"\\red255\\green255\\blue255;" // 33 
		"\\red0\\green0\\blue0;" //34
		"\\red31\\green16\\blue0;" //35
		"\\red63\\green32\\blue0;" //36
		"\\red95\\green48\\blue0;" //37
		"\\red127\\green64\\blue0;" //38
		"\\red159\\green80\\blue0;" //39
		"\\red191\\green96\\blue0;" //40
		"\\red223\\green112\\blue0;" //41
		"\\red255\\green128\\blue0;" // 42 
		"\\red255\\green143\\blue31;" // 43 
		"\\red255\\green159\\blue63;" // 44 
		"\\red255\\green175\\blue95;" // 45 
		"\\red255\\green191\\blue127;" // 46 
		"\\red255\\green207\\blue159;" // 47 
		"\\red255\\green223\\blue191;" // 48 
		"\\red255\\green239\\blue223;" // 49 
		"\\red255\\green255\\blue255;" // 50 
		"\\red0\\green0\\blue0;" //51
		"\\red31\\green31\\blue0;" //52
		"\\red63\\green63\\blue0;" //53
		"\\red95\\green95\\blue0;" //54
		"\\red127\\green127\\blue0;" //55
		"\\red159\\green159\\blue0;" //56
		"\\red191\\green191\\blue0;" //57
		"\\red223\\green223\\blue0;" //58
		"\\red255\\green255\\blue0;" // 59 
		"\\red255\\green255\\blue31;" // 60 
		"\\red255\\green255\\blue63;" // 61 
		"\\red255\\green255\\blue95;" // 62 
		"\\red255\\green255\\blue127;" // 63 
		"\\red255\\green255\\blue159;" // 64 
		"\\red255\\green255\\blue191;" // 65 
		"\\red255\\green255\\blue223;" // 66 
		"\\red255\\green255\\blue255;" // 67 
		"\\red0\\green0\\blue0;" //68
		"\\red16\\green31\\blue0;" //69
		"\\red32\\green63\\blue0;" //70
		"\\red48\\green95\\blue0;" //71
		"\\red64\\green127\\blue0;" //72
		"\\red80\\green159\\blue0;" //73
		"\\red96\\green191\\blue0;" //74
		"\\red112\\green223\\blue0;" //75
		"\\red128\\green255\\blue0;" // 76 
		"\\red143\\green255\\blue31;" // 77 
		"\\red159\\green255\\blue63;" // 78 
		"\\red175\\green255\\blue95;" // 79 
		"\\red191\\green255\\blue127;" // 80 
		"\\red207\\green255\\blue159;" // 81 
		"\\red223\\green255\\blue191;" // 82 
		"\\red239\\green255\\blue223;" // 83 
		"\\red255\\green255\\blue255;" // 84 
		"\\red0\\green0\\blue0;" //85
		"\\red0\\green31\\blue0;" //86
		"\\red0\\green63\\blue0;" //87
		"\\red0\\green95\\blue0;" //88
		"\\red0\\green127\\blue0;" //89
		"\\red0\\green159\\blue0;" //90
		"\\red0\\green191\\blue0;" //91
		"\\red0\\green223\\blue0;" //92
		"\\red0\\green255\\blue0;" // 93 
		"\\red31\\green255\\blue31;" // 94 
		"\\red63\\green255\\blue63;" // 95 
		"\\red95\\green255\\blue95;" // 96 
		"\\red127\\green255\\blue127;" // 97 
		"\\red159\\green255\\blue159;" // 98 
		"\\red191\\green255\\blue191;" // 99 
		"\\red223\\green255\\blue223;" // 100 
		"\\red255\\green255\\blue255;" // 101 
		"\\red0\\green0\\blue0;" //102
		"\\red0\\green31\\blue16;" //103
		"\\red0\\green63\\blue32;" //104
		"\\red0\\green95\\blue48;" //105
		"\\red0\\green127\\blue64;" //106
		"\\red0\\green159\\blue80;" //107
		"\\red0\\green191\\blue96;" //108
		"\\red0\\green223\\blue112;" //109
		"\\red0\\green255\\blue128;" // 110 
		"\\red31\\green255\\blue143;" // 111 
		"\\red63\\green255\\blue159;" // 112 
		"\\red95\\green255\\blue175;" // 113 
		"\\red127\\green255\\blue191;" // 114 
		"\\red159\\green255\\blue207;" // 115 
		"\\red191\\green255\\blue223;" // 116 
		"\\red223\\green255\\blue239;" // 117 
		"\\red255\\green255\\blue255;" // 118 
		"\\red0\\green0\\blue0;" //119
		"\\red0\\green31\\blue31;" //120
		"\\red0\\green63\\blue63;" //121
		"\\red0\\green95\\blue95;" //122
		"\\red0\\green127\\blue127;" //123
		"\\red0\\green159\\blue159;" //124
		"\\red0\\green191\\blue191;" //125
		"\\red0\\green223\\blue223;" //126
		"\\red0\\green255\\blue255;" // 127 
		"\\red31\\green255\\blue255;" // 128 
		"\\red63\\green255\\blue255;" // 129 
		"\\red95\\green255\\blue255;" // 130 
		"\\red127\\green255\\blue255;" // 131 
		"\\red159\\green255\\blue255;" // 132 
		"\\red191\\green255\\blue255;" // 133 
		"\\red223\\green255\\blue255;" // 134 
		"\\red255\\green255\\blue255;" // 135 
		"\\red0\\green0\\blue0;" //136
		"\\red0\\green16\\blue31;" //137
		"\\red0\\green32\\blue63;" //138
		"\\red0\\green48\\blue95;" //139
		"\\red0\\green64\\blue127;" //140
		"\\red0\\green80\\blue159;" //141
		"\\red0\\green96\\blue191;" //142
		"\\red0\\green112\\blue223;" //143
		"\\red0\\green128\\blue255;" // 144 
		"\\red31\\green143\\blue255;" // 145 
		"\\red63\\green159\\blue255;" // 146 
		"\\red95\\green175\\blue255;" // 147 
		"\\red127\\green191\\blue255;" // 148 
		"\\red159\\green207\\blue255;" // 149 
		"\\red191\\green223\\blue255;" // 150 
		"\\red223\\green239\\blue255;" // 151 
		"\\red255\\green255\\blue255;" // 152 
		"\\red0\\green0\\blue0;" //153
		"\\red0\\green0\\blue31;" //154
		"\\red0\\green0\\blue63;" //155
		"\\red0\\green0\\blue95;" //156
		"\\red0\\green0\\blue127;" //157
		"\\red0\\green0\\blue159;" //158
		"\\red0\\green0\\blue191;" //159
		"\\red0\\green0\\blue223;" //160
		"\\red0\\green0\\blue255;" // 161 
		"\\red31\\green31\\blue255;" // 162 
		"\\red63\\green63\\blue255;" // 163 
		"\\red95\\green95\\blue255;" // 164 
		"\\red127\\green127\\blue255;" // 165 
		"\\red159\\green159\\blue255;" // 166 
		"\\red191\\green191\\blue255;" // 167 
		"\\red223\\green223\\blue255;" // 168 
		"\\red255\\green255\\blue255;" // 169 
		"\\red0\\green0\\blue0;" //170
		"\\red16\\green0\\blue31;" //171
		"\\red32\\green0\\blue63;" //172
		"\\red48\\green0\\blue95;" //173
		"\\red64\\green0\\blue127;" //174
		"\\red80\\green0\\blue159;" //175
		"\\red96\\green0\\blue191;" //176
		"\\red112\\green0\\blue223;" //177
		"\\red128\\green0\\blue255;" // 178 
		"\\red143\\green31\\blue255;" // 179 
		"\\red159\\green63\\blue255;" // 180 
		"\\red175\\green95\\blue255;" // 181 
		"\\red191\\green127\\blue255;" // 182 
		"\\red207\\green159\\blue255;" // 183 
		"\\red223\\green191\\blue255;" // 184 
		"\\red239\\green223\\blue255;" // 185 
		"\\red255\\green255\\blue255;" // 186 
		"\\red0\\green0\\blue0;" //187
		"\\red31\\green0\\blue31;" //188
		"\\red63\\green0\\blue63;" //189
		"\\red95\\green0\\blue95;" //190
		"\\red127\\green0\\blue127;" //191
		"\\red159\\green0\\blue159;" //192
		"\\red191\\green0\\blue191;" //193
		"\\red223\\green0\\blue223;" //194
		"\\red255\\green0\\blue255;" // 195 
		"\\red255\\green31\\blue255;" // 196 
		"\\red255\\green63\\blue255;" // 197 
		"\\red255\\green95\\blue255;" // 198 
		"\\red255\\green127\\blue255;" // 199 
		"\\red255\\green159\\blue255;" // 200 
		"\\red255\\green191\\blue255;" // 201 
		"\\red255\\green223\\blue255;" // 202 
		"\\red255\\green255\\blue255;" // 203 
		"\\red0\\green0\\blue0;" //204
		"\\red31\\green0\\blue16;" //205
		"\\red63\\green0\\blue32;" //206
		"\\red95\\green0\\blue48;" //207
		"\\red127\\green0\\blue64;" //208
		"\\red159\\green0\\blue80;" //209
		"\\red191\\green0\\blue96;" //210
		"\\red223\\green0\\blue112;" //211
		"\\red255\\green0\\blue128;" // 212 
		"\\red255\\green31\\blue143;" // 213 
		"\\red255\\green63\\blue159;" // 214 
		"\\red255\\green95\\blue175;" // 215 
		"\\red255\\green127\\blue191;" // 216 
		"\\red255\\green159\\blue207;" // 217 
		"\\red255\\green191\\blue223;" // 218 
		"\\red255\\green223\\blue239;" // 219 
		"\\red255\\green255\\blue255;" // 220 
	;
}