#pragma once

#include "TString.h"

enum GCStringBuilderTextFormat {
	SBTF_TEXT,
	SBTF_RTF,
	SBTF_HTML,
	SBTF_XML
};

class GCStringBuilder
{
public:
	int fontSizeH1;
	int fontSizeH2;
	int fontSizeText;
	int fontSizeNote;

	GCStringBuilderTextFormat Format;
	TString * Target;

	GCStringBuilder(void);
	~GCStringBuilder(void);
	void AppendString(const char * text);
	void AppendBoldString(const char * text);
	void AppendLine() { AppendLine(""); };
	void AppendLine(const char * text);
	void initWithString(TString * str);
	void AppendHeader1(const char * text);
	void AppendHeader2(const char * text);
	void AppendDocumentHeader(void);
	void AppendDocumentTail(void);
	void AppendNote(void);
	void AppendSeparatorWithWidth(int width);
	void AppendTwoColumnText(const char *pText, const char * pText2);



	static void GCStringBuilder::AppendRtfHeader(TString * m_text);
	static void AppendRtfColorTable(TString * str);

};

