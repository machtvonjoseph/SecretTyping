; ModuleID = '../incremental_proof/scratchpad.cpp'
source_filename = "../incremental_proof/scratchpad.cpp"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%"class.std::ios_base::Init" = type { i8 }
%"class.std::runtime_error" = type { %"class.std::exception", %"struct.std::__cow_string" }
%"class.std::exception" = type { i32 (...)** }
%"struct.std::__cow_string" = type { %union.anon }
%union.anon = type { i8* }
%class.numa = type { i32 }
%class.numa.0 = type { i32, %class.numa.1 }
%class.numa.1 = type { i32* }
%class.NumaAllocator = type { i8 }
%class.NumaAllocator.2 = type { i8 }
%class.MyVector = type { i32* }
%"class.std::bad_alloc" = type { %"class.std::exception" }

$_ZN4numaIiLi3E13NumaAllocatorvEnwEm = comdat any

$_ZN4numaIiLi3E13NumaAllocatorvEC2Ev = comdat any

$_ZN4numaI8MyVectorLi3E13NumaAllocatorvEnwEm = comdat any

$_ZN4numaI8MyVectorLi3E13NumaAllocatorvEC2Eii = comdat any

$_ZN13NumaAllocatorIiLi3EEC2Ev = comdat any

$_ZN13NumaAllocatorIiLi3EE8allocateEm = comdat any

$_ZNSt9bad_allocC2Ev = comdat any

$_ZNSt9exceptionC2Ev = comdat any

$_ZN4numaIPiLi3E13NumaAllocatorvEC2Ev = comdat any

$_ZN4numaIPiLi3E13NumaAllocatorvEC2ES0_ = comdat any

$_ZN4numaIPiLi3E13NumaAllocatorvEcvRS0_Ev = comdat any

$_ZN13NumaAllocatorI8MyVectorLi3EEC2Ev = comdat any

$_ZN13NumaAllocatorI8MyVectorLi3EE8allocateEm = comdat any

@_ZStL8__ioinit = internal global %"class.std::ios_base::Init" zeroinitializer, align 1
@__dso_handle = external hidden global i8
@.str = private unnamed_addr constant [18 x i8] c"move_pages failed\00", align 1
@_ZTISt13runtime_error = external constant i8*
@_ZTISt9bad_alloc = external constant i8*
@_ZTVSt9bad_alloc = external unnamed_addr constant { [5 x i8*] }, align 8
@_ZTVSt9exception = external unnamed_addr constant { [5 x i8*] }, align 8
@llvm.global_ctors = appending global [1 x { i32, void ()*, i8* }] [{ i32, void ()*, i8* } { i32 65535, void ()* @_GLOBAL__sub_I_scratchpad.cpp, i8* null }]

; Function Attrs: noinline uwtable
define internal void @__cxx_global_var_init() #0 section ".text.startup" {
entry:
  call void @_ZNSt8ios_base4InitC1Ev(%"class.std::ios_base::Init"* noundef nonnull align 1 dereferenceable(1) @_ZStL8__ioinit)
  %0 = call i32 @__cxa_atexit(void (i8*)* bitcast (void (%"class.std::ios_base::Init"*)* @_ZNSt8ios_base4InitD1Ev to void (i8*)*), i8* getelementptr inbounds (%"class.std::ios_base::Init", %"class.std::ios_base::Init"* @_ZStL8__ioinit, i32 0, i32 0), i8* @__dso_handle) #3
  ret void
}

declare void @_ZNSt8ios_base4InitC1Ev(%"class.std::ios_base::Init"* noundef nonnull align 1 dereferenceable(1)) unnamed_addr #1

; Function Attrs: nounwind
declare void @_ZNSt8ios_base4InitD1Ev(%"class.std::ios_base::Init"* noundef nonnull align 1 dereferenceable(1)) unnamed_addr #2

; Function Attrs: nounwind
declare i32 @__cxa_atexit(void (i8*)*, i8*, i8*) #3

; Function Attrs: mustprogress noinline optnone uwtable
define dso_local noundef i32 @_Z16get_numa_node_idPv(i8* noundef %ptr) #4 personality i8* bitcast (i32 (...)* @__gxx_personality_v0 to i8*) {
entry:
  %ptr.addr = alloca i8*, align 8
  %status = alloca [1 x i32], align 4
  %ret_code = alloca i32, align 4
  %exn.slot = alloca i8*, align 8
  %ehselector.slot = alloca i32, align 4
  store i8* %ptr, i8** %ptr.addr, align 8
  %arrayidx = getelementptr inbounds [1 x i32], [1 x i32]* %status, i64 0, i64 0
  store i32 -1, i32* %arrayidx, align 4
  %arraydecay = getelementptr inbounds [1 x i32], [1 x i32]* %status, i64 0, i64 0
  %call = call i64 @move_pages(i32 noundef 0, i64 noundef 1, i8** noundef %ptr.addr, i32* noundef null, i32* noundef %arraydecay, i32 noundef 0)
  %conv = trunc i64 %call to i32
  store i32 %conv, i32* %ret_code, align 4
  %0 = load i32, i32* %ret_code, align 4
  %cmp = icmp ne i32 %0, 0
  br i1 %cmp, label %if.then, label %if.end

if.then:                                          ; preds = %entry
  %exception = call i8* @__cxa_allocate_exception(i64 16) #3
  %1 = bitcast i8* %exception to %"class.std::runtime_error"*
  invoke void @_ZNSt13runtime_errorC1EPKc(%"class.std::runtime_error"* noundef nonnull align 8 dereferenceable(16) %1, i8* noundef getelementptr inbounds ([18 x i8], [18 x i8]* @.str, i64 0, i64 0))
          to label %invoke.cont unwind label %lpad

invoke.cont:                                      ; preds = %if.then
  call void @__cxa_throw(i8* %exception, i8* bitcast (i8** @_ZTISt13runtime_error to i8*), i8* bitcast (void (%"class.std::runtime_error"*)* @_ZNSt13runtime_errorD1Ev to i8*)) #13
  unreachable

lpad:                                             ; preds = %if.then
  %2 = landingpad { i8*, i32 }
          cleanup
  %3 = extractvalue { i8*, i32 } %2, 0
  store i8* %3, i8** %exn.slot, align 8
  %4 = extractvalue { i8*, i32 } %2, 1
  store i32 %4, i32* %ehselector.slot, align 4
  call void @__cxa_free_exception(i8* %exception) #3
  br label %eh.resume

if.end:                                           ; preds = %entry
  %arrayidx1 = getelementptr inbounds [1 x i32], [1 x i32]* %status, i64 0, i64 0
  %5 = load i32, i32* %arrayidx1, align 4
  ret i32 %5

eh.resume:                                        ; preds = %lpad
  %exn = load i8*, i8** %exn.slot, align 8
  %sel = load i32, i32* %ehselector.slot, align 4
  %lpad.val = insertvalue { i8*, i32 } undef, i8* %exn, 0
  %lpad.val2 = insertvalue { i8*, i32 } %lpad.val, i32 %sel, 1
  resume { i8*, i32 } %lpad.val2
}

declare i64 @move_pages(i32 noundef, i64 noundef, i8** noundef, i32* noundef, i32* noundef, i32 noundef) #1

declare i8* @__cxa_allocate_exception(i64)

declare void @_ZNSt13runtime_errorC1EPKc(%"class.std::runtime_error"* noundef nonnull align 8 dereferenceable(16), i8* noundef) unnamed_addr #1

declare i32 @__gxx_personality_v0(...)

declare void @__cxa_free_exception(i8*)

; Function Attrs: nounwind
declare void @_ZNSt13runtime_errorD1Ev(%"class.std::runtime_error"* noundef nonnull align 8 dereferenceable(16)) unnamed_addr #2

declare void @__cxa_throw(i8*, i8*, i8*)

; Function Attrs: mustprogress noinline norecurse optnone uwtable
define dso_local noundef i32 @main() #5 personality i8* bitcast (i32 (...)* @__gxx_personality_v0 to i8*) {
entry:
  %v2 = alloca %class.numa*, align 8
  %exn.slot = alloca i8*, align 8
  %ehselector.slot = alloca i32, align 4
  %v = alloca %class.numa.0*, align 8
  %call = call noundef i8* @_ZN4numaIiLi3E13NumaAllocatorvEnwEm(i64 noundef 4)
  %0 = bitcast i8* %call to %class.numa*
  invoke void @_ZN4numaIiLi3E13NumaAllocatorvEC2Ev(%class.numa* noundef nonnull align 4 dereferenceable(4) %0)
          to label %invoke.cont unwind label %lpad

invoke.cont:                                      ; preds = %entry
  store %class.numa* %0, %class.numa** %v2, align 8
  %call1 = call noundef i8* @_ZN4numaI8MyVectorLi3E13NumaAllocatorvEnwEm(i64 noundef 16)
  %1 = bitcast i8* %call1 to %class.numa.0*
  invoke void @_ZN4numaI8MyVectorLi3E13NumaAllocatorvEC2Eii(%class.numa.0* noundef nonnull align 8 dereferenceable(16) %1, i32 noundef 10, i32 noundef 3)
          to label %invoke.cont3 unwind label %lpad2

invoke.cont3:                                     ; preds = %invoke.cont
  store %class.numa.0* %1, %class.numa.0** %v, align 8
  ret i32 0

lpad:                                             ; preds = %entry
  %2 = landingpad { i8*, i32 }
          cleanup
  %3 = extractvalue { i8*, i32 } %2, 0
  store i8* %3, i8** %exn.slot, align 8
  %4 = extractvalue { i8*, i32 } %2, 1
  store i32 %4, i32* %ehselector.slot, align 4
  call void @_ZdlPv(i8* noundef %call) #14
  br label %eh.resume

lpad2:                                            ; preds = %invoke.cont
  %5 = landingpad { i8*, i32 }
          cleanup
  %6 = extractvalue { i8*, i32 } %5, 0
  store i8* %6, i8** %exn.slot, align 8
  %7 = extractvalue { i8*, i32 } %5, 1
  store i32 %7, i32* %ehselector.slot, align 4
  call void @_ZdlPv(i8* noundef %call1) #14
  br label %eh.resume

eh.resume:                                        ; preds = %lpad2, %lpad
  %exn = load i8*, i8** %exn.slot, align 8
  %sel = load i32, i32* %ehselector.slot, align 4
  %lpad.val = insertvalue { i8*, i32 } undef, i8* %exn, 0
  %lpad.val4 = insertvalue { i8*, i32 } %lpad.val, i32 %sel, 1
  resume { i8*, i32 } %lpad.val4
}

; Function Attrs: mustprogress noinline optnone uwtable
define linkonce_odr dso_local noundef i8* @_ZN4numaIiLi3E13NumaAllocatorvEnwEm(i64 noundef %sz) #4 comdat align 2 {
entry:
  %sz.addr = alloca i64, align 8
  %alloc = alloca %class.NumaAllocator, align 1
  store i64 %sz, i64* %sz.addr, align 8
  call void @_ZN13NumaAllocatorIiLi3EEC2Ev(%class.NumaAllocator* noundef nonnull align 1 dereferenceable(1) %alloc) #3
  %0 = load i64, i64* %sz.addr, align 8
  %call = call noundef i32* @_ZN13NumaAllocatorIiLi3EE8allocateEm(%class.NumaAllocator* noundef nonnull align 1 dereferenceable(1) %alloc, i64 noundef %0)
  %1 = bitcast i32* %call to i8*
  ret i8* %1
}

; Function Attrs: noinline optnone uwtable
define linkonce_odr dso_local void @_ZN4numaIiLi3E13NumaAllocatorvEC2Ev(%class.numa* noundef nonnull align 4 dereferenceable(4) %this) unnamed_addr #6 comdat align 2 {
entry:
  %this.addr.i = alloca %class.numa*, align 8
  %data.addr.i = alloca i32, align 4
  %this.addr = alloca %class.numa*, align 8
  store %class.numa* %this, %class.numa** %this.addr, align 8
  %this1 = load %class.numa*, %class.numa** %this.addr, align 8
  store %class.numa* %this1, %class.numa** %this.addr.i, align 8
  store i32 0, i32* %data.addr.i, align 4
  %this1.i = load %class.numa*, %class.numa** %this.addr.i, align 8
  %0 = load i32, i32* %data.addr.i, align 4
  %contents.i = getelementptr inbounds %class.numa, %class.numa* %this1.i, i32 0, i32 0
  store i32 %0, i32* %contents.i, align 4
  ret void
}

; Function Attrs: nobuiltin nounwind
declare void @_ZdlPv(i8* noundef) #7

; Function Attrs: mustprogress noinline optnone uwtable
define linkonce_odr dso_local noundef i8* @_ZN4numaI8MyVectorLi3E13NumaAllocatorvEnwEm(i64 noundef %count) #4 comdat align 2 {
entry:
  %count.addr = alloca i64, align 8
  %alloc = alloca %class.NumaAllocator.2, align 1
  store i64 %count, i64* %count.addr, align 8
  call void @_ZN13NumaAllocatorI8MyVectorLi3EEC2Ev(%class.NumaAllocator.2* noundef nonnull align 1 dereferenceable(1) %alloc) #3
  %0 = load i64, i64* %count.addr, align 8
  %call = call noundef %class.MyVector* @_ZN13NumaAllocatorI8MyVectorLi3EE8allocateEm(%class.NumaAllocator.2* noundef nonnull align 1 dereferenceable(1) %alloc, i64 noundef %0)
  %1 = bitcast %class.MyVector* %call to i8*
  ret i8* %1
}

; Function Attrs: noinline optnone uwtable
define linkonce_odr dso_local void @_ZN4numaI8MyVectorLi3E13NumaAllocatorvEC2Eii(%class.numa.0* noundef nonnull align 8 dereferenceable(16) %this, i32 noundef %sz, i32 noundef %val) unnamed_addr #6 comdat align 2 {
entry:
  %this.addr = alloca %class.numa.0*, align 8
  %sz.addr = alloca i32, align 4
  %val.addr = alloca i32, align 4
  %ref.tmp = alloca %class.numa.1, align 8
  %alis = alloca i32*, align 8
  store %class.numa.0* %this, %class.numa.0** %this.addr, align 8
  store i32 %sz, i32* %sz.addr, align 4
  store i32 %val, i32* %val.addr, align 4
  %this1 = load %class.numa.0*, %class.numa.0** %this.addr, align 8
  %node_id = getelementptr inbounds %class.numa.0, %class.numa.0* %this1, i32 0, i32 0
  store i32 3, i32* %node_id, align 8
  %my_data = getelementptr inbounds %class.numa.0, %class.numa.0* %this1, i32 0, i32 1
  call void @_ZN4numaIPiLi3E13NumaAllocatorvEC2Ev(%class.numa.1* noundef nonnull align 8 dereferenceable(8) %my_data)
  %0 = load i32, i32* %sz.addr, align 4
  %conv = sext i32 %0 to i64
  %1 = call { i64, i1 } @llvm.umul.with.overflow.i64(i64 %conv, i64 4)
  %2 = extractvalue { i64, i1 } %1, 1
  %3 = extractvalue { i64, i1 } %1, 0
  %4 = select i1 %2, i64 -1, i64 %3
  %call = call noalias noundef nonnull i8* @_Znam(i64 noundef %4) #15
  %5 = bitcast i8* %call to i32*
  call void @_ZN4numaIPiLi3E13NumaAllocatorvEC2ES0_(%class.numa.1* noundef nonnull align 8 dereferenceable(8) %ref.tmp, i32* noundef %5)
  %my_data2 = getelementptr inbounds %class.numa.0, %class.numa.0* %this1, i32 0, i32 1
  %6 = bitcast %class.numa.1* %my_data2 to i8*
  %7 = bitcast %class.numa.1* %ref.tmp to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 8 %6, i8* align 8 %7, i64 8, i1 false)
  %my_data3 = getelementptr inbounds %class.numa.0, %class.numa.0* %this1, i32 0, i32 1
  %call4 = call noundef nonnull align 8 dereferenceable(8) i32** @_ZN4numaIPiLi3E13NumaAllocatorvEcvRS0_Ev(%class.numa.1* noundef nonnull align 8 dereferenceable(8) %my_data3)
  %8 = load i32*, i32** %call4, align 8
  store i32* %8, i32** %alis, align 8
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define linkonce_odr dso_local void @_ZN13NumaAllocatorIiLi3EEC2Ev(%class.NumaAllocator* noundef nonnull align 1 dereferenceable(1) %this) unnamed_addr #8 comdat align 2 {
entry:
  %this.addr = alloca %class.NumaAllocator*, align 8
  store %class.NumaAllocator* %this, %class.NumaAllocator** %this.addr, align 8
  %this1 = load %class.NumaAllocator*, %class.NumaAllocator** %this.addr, align 8
  ret void
}

; Function Attrs: mustprogress noinline optnone uwtable
define linkonce_odr dso_local noundef i32* @_ZN13NumaAllocatorIiLi3EE8allocateEm(%class.NumaAllocator* noundef nonnull align 1 dereferenceable(1) %this, i64 noundef %n) #4 comdat align 2 {
entry:
  %this.addr = alloca %class.NumaAllocator*, align 8
  %n.addr = alloca i64, align 8
  %p = alloca i8*, align 8
  store %class.NumaAllocator* %this, %class.NumaAllocator** %this.addr, align 8
  store i64 %n, i64* %n.addr, align 8
  %this1 = load %class.NumaAllocator*, %class.NumaAllocator** %this.addr, align 8
  %0 = load i64, i64* %n.addr, align 8
  %mul = mul i64 %0, 4
  %call = call i8* @numa_alloc_onnode(i64 noundef %mul, i32 noundef 3)
  store i8* %call, i8** %p, align 8
  %1 = load i8*, i8** %p, align 8
  %cmp = icmp eq i8* %1, null
  br i1 %cmp, label %if.then, label %if.end

if.then:                                          ; preds = %entry
  %exception = call i8* @__cxa_allocate_exception(i64 8) #3
  %2 = bitcast i8* %exception to %"class.std::bad_alloc"*
  call void @_ZNSt9bad_allocC2Ev(%"class.std::bad_alloc"* noundef nonnull align 8 dereferenceable(8) %2) #3
  call void @__cxa_throw(i8* %exception, i8* bitcast (i8** @_ZTISt9bad_alloc to i8*), i8* bitcast (void (%"class.std::bad_alloc"*)* @_ZNSt9bad_allocD1Ev to i8*)) #13
  unreachable

if.end:                                           ; preds = %entry
  %3 = load i8*, i8** %p, align 8
  %4 = bitcast i8* %3 to i32*
  ret i32* %4
}

declare i8* @numa_alloc_onnode(i64 noundef, i32 noundef) #1

; Function Attrs: noinline nounwind optnone uwtable
define linkonce_odr dso_local void @_ZNSt9bad_allocC2Ev(%"class.std::bad_alloc"* noundef nonnull align 8 dereferenceable(8) %this) unnamed_addr #8 comdat align 2 {
entry:
  %this.addr = alloca %"class.std::bad_alloc"*, align 8
  store %"class.std::bad_alloc"* %this, %"class.std::bad_alloc"** %this.addr, align 8
  %this1 = load %"class.std::bad_alloc"*, %"class.std::bad_alloc"** %this.addr, align 8
  %0 = bitcast %"class.std::bad_alloc"* %this1 to %"class.std::exception"*
  call void @_ZNSt9exceptionC2Ev(%"class.std::exception"* noundef nonnull align 8 dereferenceable(8) %0) #3
  %1 = bitcast %"class.std::bad_alloc"* %this1 to i32 (...)***
  store i32 (...)** bitcast (i8** getelementptr inbounds ({ [5 x i8*] }, { [5 x i8*] }* @_ZTVSt9bad_alloc, i32 0, inrange i32 0, i32 2) to i32 (...)**), i32 (...)*** %1, align 8
  ret void
}

; Function Attrs: nounwind
declare void @_ZNSt9bad_allocD1Ev(%"class.std::bad_alloc"* noundef nonnull align 8 dereferenceable(8)) unnamed_addr #2

; Function Attrs: noinline nounwind optnone uwtable
define linkonce_odr dso_local void @_ZNSt9exceptionC2Ev(%"class.std::exception"* noundef nonnull align 8 dereferenceable(8) %this) unnamed_addr #8 comdat align 2 {
entry:
  %this.addr = alloca %"class.std::exception"*, align 8
  store %"class.std::exception"* %this, %"class.std::exception"** %this.addr, align 8
  %this1 = load %"class.std::exception"*, %"class.std::exception"** %this.addr, align 8
  %0 = bitcast %"class.std::exception"* %this1 to i32 (...)***
  store i32 (...)** bitcast (i8** getelementptr inbounds ({ [5 x i8*] }, { [5 x i8*] }* @_ZTVSt9exception, i32 0, inrange i32 0, i32 2) to i32 (...)**), i32 (...)*** %0, align 8
  ret void
}

; Function Attrs: noinline optnone uwtable
define linkonce_odr dso_local void @_ZN4numaIPiLi3E13NumaAllocatorvEC2Ev(%class.numa.1* noundef nonnull align 8 dereferenceable(8) %this) unnamed_addr #6 comdat align 2 {
entry:
  %this.addr.i = alloca %class.numa.1*, align 8
  %data.addr.i = alloca i32*, align 8
  %this.addr = alloca %class.numa.1*, align 8
  store %class.numa.1* %this, %class.numa.1** %this.addr, align 8
  %this1 = load %class.numa.1*, %class.numa.1** %this.addr, align 8
  store %class.numa.1* %this1, %class.numa.1** %this.addr.i, align 8
  store i32* null, i32** %data.addr.i, align 8
  %this1.i = load %class.numa.1*, %class.numa.1** %this.addr.i, align 8
  %0 = load i32*, i32** %data.addr.i, align 8
  %contents.i = getelementptr inbounds %class.numa.1, %class.numa.1* %this1.i, i32 0, i32 0
  store i32* %0, i32** %contents.i, align 8
  ret void
}

; Function Attrs: nofree nosync nounwind readnone speculatable willreturn
declare { i64, i1 } @llvm.umul.with.overflow.i64(i64, i64) #9

; Function Attrs: nobuiltin allocsize(0)
declare noundef nonnull i8* @_Znam(i64 noundef) #10

; Function Attrs: noinline nounwind optnone uwtable
define linkonce_odr dso_local void @_ZN4numaIPiLi3E13NumaAllocatorvEC2ES0_(%class.numa.1* noundef nonnull align 8 dereferenceable(8) %this, i32* noundef %data) unnamed_addr #8 comdat align 2 {
entry:
  %this.addr.i = alloca %class.numa.1*, align 8
  %data.addr.i = alloca i32*, align 8
  %this.addr = alloca %class.numa.1*, align 8
  %data.addr = alloca i32*, align 8
  store %class.numa.1* %this, %class.numa.1** %this.addr, align 8
  store i32* %data, i32** %data.addr, align 8
  %this1 = load %class.numa.1*, %class.numa.1** %this.addr, align 8
  %0 = load i32*, i32** %data.addr, align 8
  store %class.numa.1* %this1, %class.numa.1** %this.addr.i, align 8
  store i32* %0, i32** %data.addr.i, align 8
  %this1.i = load %class.numa.1*, %class.numa.1** %this.addr.i, align 8
  %1 = load i32*, i32** %data.addr.i, align 8
  %contents.i = getelementptr inbounds %class.numa.1, %class.numa.1* %this1.i, i32 0, i32 0
  store i32* %1, i32** %contents.i, align 8
  ret void
}

; Function Attrs: argmemonly nofree nounwind willreturn
declare void @llvm.memcpy.p0i8.p0i8.i64(i8* noalias nocapture writeonly, i8* noalias nocapture readonly, i64, i1 immarg) #11

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define linkonce_odr dso_local noundef nonnull align 8 dereferenceable(8) i32** @_ZN4numaIPiLi3E13NumaAllocatorvEcvRS0_Ev(%class.numa.1* noundef nonnull align 8 dereferenceable(8) %this) #12 comdat align 2 {
entry:
  %this.addr = alloca %class.numa.1*, align 8
  store %class.numa.1* %this, %class.numa.1** %this.addr, align 8
  %this1 = load %class.numa.1*, %class.numa.1** %this.addr, align 8
  %contents = getelementptr inbounds %class.numa.1, %class.numa.1* %this1, i32 0, i32 0
  ret i32** %contents
}

; Function Attrs: noinline nounwind optnone uwtable
define linkonce_odr dso_local void @_ZN13NumaAllocatorI8MyVectorLi3EEC2Ev(%class.NumaAllocator.2* noundef nonnull align 1 dereferenceable(1) %this) unnamed_addr #8 comdat align 2 {
entry:
  %this.addr = alloca %class.NumaAllocator.2*, align 8
  store %class.NumaAllocator.2* %this, %class.NumaAllocator.2** %this.addr, align 8
  %this1 = load %class.NumaAllocator.2*, %class.NumaAllocator.2** %this.addr, align 8
  ret void
}

; Function Attrs: mustprogress noinline optnone uwtable
define linkonce_odr dso_local noundef %class.MyVector* @_ZN13NumaAllocatorI8MyVectorLi3EE8allocateEm(%class.NumaAllocator.2* noundef nonnull align 1 dereferenceable(1) %this, i64 noundef %n) #4 comdat align 2 {
entry:
  %this.addr = alloca %class.NumaAllocator.2*, align 8
  %n.addr = alloca i64, align 8
  %p = alloca i8*, align 8
  store %class.NumaAllocator.2* %this, %class.NumaAllocator.2** %this.addr, align 8
  store i64 %n, i64* %n.addr, align 8
  %this1 = load %class.NumaAllocator.2*, %class.NumaAllocator.2** %this.addr, align 8
  %0 = load i64, i64* %n.addr, align 8
  %mul = mul i64 %0, 8
  %call = call i8* @numa_alloc_onnode(i64 noundef %mul, i32 noundef 3)
  store i8* %call, i8** %p, align 8
  %1 = load i8*, i8** %p, align 8
  %cmp = icmp eq i8* %1, null
  br i1 %cmp, label %if.then, label %if.end

if.then:                                          ; preds = %entry
  %exception = call i8* @__cxa_allocate_exception(i64 8) #3
  %2 = bitcast i8* %exception to %"class.std::bad_alloc"*
  call void @_ZNSt9bad_allocC2Ev(%"class.std::bad_alloc"* noundef nonnull align 8 dereferenceable(8) %2) #3
  call void @__cxa_throw(i8* %exception, i8* bitcast (i8** @_ZTISt9bad_alloc to i8*), i8* bitcast (void (%"class.std::bad_alloc"*)* @_ZNSt9bad_allocD1Ev to i8*)) #13
  unreachable

if.end:                                           ; preds = %entry
  %3 = load i8*, i8** %p, align 8
  %4 = bitcast i8* %3 to %class.MyVector*
  ret %class.MyVector* %4
}

; Function Attrs: noinline uwtable
define internal void @_GLOBAL__sub_I_scratchpad.cpp() #0 section ".text.startup" {
entry:
  call void @__cxx_global_var_init()
  ret void
}

attributes #0 = { noinline uwtable "frame-pointer"="all" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #1 = { "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #2 = { nounwind "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #3 = { nounwind }
attributes #4 = { mustprogress noinline optnone uwtable "frame-pointer"="all" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #5 = { mustprogress noinline norecurse optnone uwtable "frame-pointer"="all" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #6 = { noinline optnone uwtable "frame-pointer"="all" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #7 = { nobuiltin nounwind "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #8 = { noinline nounwind optnone uwtable "frame-pointer"="all" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #9 = { nofree nosync nounwind readnone speculatable willreturn }
attributes #10 = { nobuiltin allocsize(0) "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #11 = { argmemonly nofree nounwind willreturn }
attributes #12 = { mustprogress noinline nounwind optnone uwtable "frame-pointer"="all" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #13 = { noreturn }
attributes #14 = { builtin nounwind }
attributes #15 = { builtin allocsize(0) }

!llvm.module.flags = !{!0, !1, !2, !3, !4}
!llvm.ident = !{!5}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 7, !"PIC Level", i32 2}
!2 = !{i32 7, !"PIE Level", i32 2}
!3 = !{i32 7, !"uwtable", i32 1}
!4 = !{i32 7, !"frame-pointer", i32 2}
!5 = !{!"Ubuntu clang version 14.0.0-1ubuntu1.1"}
