// The MIT License (MIT)

// Copyright (c) 2013 lailongwei<lailongwei@126.com>
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy of 
// this software and associated documentation files (the "Software"), to deal in 
// the Software without restriction, including without limitation the rights to 
// use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of 
// the Software, and to permit persons to whom the Software is furnished to do so, 
// subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all 
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS 
// FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR 
// COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER 
// IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN 
// CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

#include "pyllbc/common/Common.h"

LLBC_EXTERN_C PyObject *_pyllbc_Event_New(PyObject *self, PyObject *args)
{
    int evId;
    PyObject *pyEv;
    if (!PyArg_ParseTuple(args, "Oi", &pyEv, &evId))
        return nullptr;

    LLBC_Event *ev = new LLBC_Event(evId, true);
    ev->SetExtData(pyEv, nullptr);

    return PyLong_FromUnsignedLongLong(reinterpret_cast<uint64>(ev));
}

LLBC_EXTERN_C PyObject *_pyllbc_Event_Del(PyObject *self, PyObject *args)
{
    LLBC_Event *ev;
    if (!PyArg_ParseTuple(args, "l", &ev))
        return nullptr;

    delete ev;

    Py_RETURN_NONE;
}

LLBC_EXTERN_C PyObject *_pyllbc_Event_GetItem(PyObject *self, PyObject *args)
{
    LLBC_Event *ev;
    const char *evParamKey;
    if (!PyArg_ParseTuple(args, "ls", &ev, &evParamKey))
        return nullptr;

    // Try find event param from constant string_key params.
    auto cit = ev->GetParams().find(LLBC_Variant(evParamKey));
    if (cit != ev->GetParams().end())
        return pyllbc_ObjUtil::Variant2Obj(cit->second);

    // Not found, set index error.
    LLBC_String errDesc;
    errDesc.format("not found event param, key:%s", evParamKey);
    pyllbc_SetError(errDesc.c_str(), LLBC_ERROR_NOT_FOUND, PyExc_IndexError);
    return nullptr;
}

LLBC_EXTERN_C PyObject *_pyllbc_Event_SetItem(PyObject *self, PyObject *args)
{
    LLBC_Event *ev;
    const char *evParamKey;
    PyObject *evParamValue;
    if (!PyArg_ParseTuple(args, "lsO", &ev, &evParamKey, &evParamValue))
        return nullptr;

    LLBC_Variant nativeEvParamValue;
    if (pyllbc_ObjUtil::Obj2Variant(evParamValue, nativeEvParamValue) != LLBC_OK)
        return nullptr;

    (*ev)[LLBC_String(evParamKey)] = nativeEvParamValue;

    Py_RETURN_NONE;
}
