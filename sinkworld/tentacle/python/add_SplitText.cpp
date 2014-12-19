#include <boost/python.hpp>
#include <boost/cstdint.hpp>

#include <string>
#include <iostream>
#include <assert.h>
#include "base.h"

// Using =======================================================================
using namespace boost::python;

#include "bbase.h"

std::string retrieval_as_str(SplitText &self, int position, int retrieveLength) {
	int lenUTF8 = retrieveLength * 3;
	SW_BYTE *bytes = new SW_BYTE[lenUTF8];
	int lenRet = self.RetrieveUTF8(position, bytes, retrieveLength);
	std::string rv((char *)bytes, lenRet);
	delete []bytes;
	return rv;
}

std::wstring retrieval_as_wstr(SplitText &self, int position, int retrieveLength) {
	int lenUTF16 = retrieveLength * 3;
	SW_SHORT *shorts = new SW_SHORT[lenUTF16];
	int lenRet = self.RetrieveUTF16(position, shorts, retrieveLength);
	std::wstring rv((wchar_t *)shorts, lenRet);
	delete []shorts;
	return rv;
}

static list retrieval_as_list(SplitText &self, int position, int retrieveLength) {
    list result;

	int *characters = new int[retrieveLength];
	int lenRet = self.RetrieveUTF32(position, characters, retrieveLength);
	for (int i=0;i<lenRet;i++) {
		result.append(characters[i]);
	}
	delete []characters;
	return result;
}

static int length_in_text(SplitText &self, Literal &lit) {
	return self.LengthInText(lit.Data(), lit.Length());
}

void add_SplitText() {
    scope* SplitText_scope = new scope(
    class_< SplitText, boost::noncopyable >("SplitText", init<  >())
        .def(init< int, int >())
        .def("XCharAt", &SplitText::XCharAt)
        .def("UnsignedByteAt", &SplitText::UnsignedByteAt)
        .def("StyleAt", &SplitText::StyleAt)
        .def("Layers", &SplitText::Layers)
        .def("StyleAtLayer", &SplitText::StyleAtLayer)
        .def("GetLayer", &SplitText::GetLayer, return_internal_reference<>())
        .def("GetSubstance", &SplitText::GetSubstance, return_internal_reference<>())
        .def("Length", &SplitText::Length)
        .def("DetachAllStyles", &SplitText::DetachAllStyles)
        .def("DetachStyles", &SplitText::DetachStyles)
        .def("AttachStyles", &SplitText::AttachStyles)
        .def("CreateStylesSized", &SplitText::CreateStylesSized, return_value_policy< manage_new_object >())
        .def("CreateStyles", &SplitText::CreateStyles, return_value_policy< manage_new_object >())
        .def("LexTo", &SplitText::LexTo)
        .def("RetrieveBytes", &SplitText::RetrieveBytes)
        .def("HashRange", &SplitText::HashRange)
        .def("Characters", &SplitText::Characters)
        .def("RetrieveUTF8", &retrieval_as_str)
        .def("RetrieveUTF16", &retrieval_as_wstr)
        .def("RetrieveUTF32", &retrieval_as_list)
        .def("LengthInText", &length_in_text)
        .def("DiscardLines", &SplitText::DiscardLines)
        .def("RecreateLines", &SplitText::RecreateLines)
        .def("CheckLines", &SplitText::CheckLines)
        .def("SetValidating", &SplitText::SetValidating)
        .def("GetValidating", &SplitText::GetValidating)
        .def("InsertBytes", (void (SplitText::*)(int, char *, int))&SplitText::InsertBytes)
        .def("InsertUTF32", &SplitText::InsertUTF32)
        .def("DeleteRange", &SplitText::DeleteRange)
        .def("Lines", &SplitText::Lines)
        .def("PositionFromLine", &SplitText::PositionFromLine)
        .def("LineEndPositionFromLine", &SplitText::LineEndPositionFromLine)
        .def("LineFromPosition", &SplitText::LineFromPosition)
        .def("AddPerLineData", &SplitText::AddPerLineData)
        .def("RemovePerLineData", &SplitText::RemovePerLineData)
        .def("SetUndoCollection", &SplitText::SetUndoCollection)
        .def("IsCollectingUndo", &SplitText::IsCollectingUndo)
        .def("BeginUndoAction", &SplitText::BeginUndoAction)
        .def("EndUndoAction", &SplitText::EndUndoAction)
        .def("DeleteUndoHistory", &SplitText::DeleteUndoHistory)
        .def("CanUndo", &SplitText::CanUndo)
        .def("StartUndo", &SplitText::StartUndo)
        .def("GetUndoStep", &SplitText::GetUndoStep, return_internal_reference<>())
        .def("PerformUndoAction", &SplitText::PerformUndoAction)
        .def("PerformUndoStep", &SplitText::PerformUndoStep)
        .def("CanRedo", &SplitText::CanRedo)
        .def("StartRedo", &SplitText::StartRedo)
        .def("GetRedoStep", &SplitText::GetRedoStep, return_internal_reference<>())
        .def("PerformRedoAction", &SplitText::PerformRedoAction)
        .def("PerformRedoStep", &SplitText::PerformRedoStep)
        .def("SetSavePoint", &SplitText::SetSavePoint)
        .def("IsSavePoint", &SplitText::IsSavePoint)
        .def("ClampPositionIntoDocument", &SplitText::ClampPositionIntoDocument)
        .def("SetEncoding", &SplitText::SetEncoding)
        .def("GetEncoding", &SplitText::GetEncoding)
        .def("DistanceNextCaret", &SplitText::DistanceNextCaret)
        .def("LenChar", &SplitText::LenChar)
        .def("PositionNext", &SplitText::PositionNext)
        .def("PositionBefore", &SplitText::PositionBefore)
        .def("CharValue", &SplitText::CharValue)
        .def("CharValueBefore", &SplitText::CharValueBefore)
        .def("Stride", &SplitText::Stride)
        .def("MovePositionOutsideChar", &SplitText::MovePositionOutsideChar)
        .staticmethod("CreateStylesSized")
        .staticmethod("CreateStyles")
    );

    scope().attr("ENCODING_BYTE") = (int)SplitText::ENCODING_BYTE;


    scope().attr("ENCODING_UTF8") = (int)SplitText::ENCODING_UTF8;


    scope().attr("ENCODING_UTF16LE") = (int)SplitText::ENCODING_UTF16LE;


    scope().attr("ENCODING_UTF16BE") = (int)SplitText::ENCODING_UTF16BE;

    delete SplitText_scope;

}
