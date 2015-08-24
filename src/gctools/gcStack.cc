/*
    File: gcStack.cc
*/

/*
Copyright (c) 2014, Christian E. Schafmeister
 
CLASP is free software; you can redistribute it and/or
modify it under the terms of the GNU Library General Public
License as published by the Free Software Foundation; either
version 2 of the License, or (at your option) any later version.
 
See directory 'clasp/licenses' for full details.
 
The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/
/* -^- */
#include <clasp/core/foundation.h>
namespace gctools {
namespace frame {
Frame::Frame(size_t numArguments,core::T_sp parent) : _NumArguments(numArguments) {
  size_t sz = FrameSize(numArguments)*sizeof(ElementType);
  this->_frameImpl = reinterpret_cast<ElementType*>(threadLocalStack()->pushFrame(sz));
  this->_frameImpl[IdxNumElements] = reinterpret_cast<core::T_O*>(numArguments);
  for (size_t i(IdxValuesArray), iEnd(IdxValuesArray + numArguments); i < iEnd; ++i) {
    this->_frameImpl[i] = gctools::tag_unbound<core::T_O *>();
  }
}

Frame::~Frame() {
  threadLocalStack()->popFrame(reinterpret_cast<void*>(this->_frameImpl));
}

};
};
