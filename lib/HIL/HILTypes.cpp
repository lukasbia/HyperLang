#include "hyper/HIL/HIL.h"
#include <string>
namespace hyper::hil {
bool isNumericType(const Value& v){return v.type=="i8"||v.type=="i16"||v.type=="i32"||v.type=="i64"||v.type=="u8"||v.type=="u16"||v.type=="u32"||v.type=="u64"||v.type=="f32"||v.type=="f64";}
bool compatibleTypes(const Value&a,const Value&b){return a.type==b.type||a.type.empty()||b.type.empty();}
}