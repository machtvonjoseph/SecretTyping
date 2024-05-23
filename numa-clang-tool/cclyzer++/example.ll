; ModuleID = './input/example.cpp'
source_filename = "./input/example.cpp"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%class.MyVector = type <{ i32*, %class.SomeClass*, %class.SomeClass, [4 x i8] }>
%class.SomeClass = type { i32 }

$_ZN8MyVectorC2Eii = comdat any

; Function Attrs: mustprogress noinline norecurse optnone uwtable
define dso_local noundef i32 @main() #0 personality i8* bitcast (i32 (...)* @__gxx_personality_v0 to i8*) {
entry:
  %v3 = alloca %class.MyVector*, align 8
  %exn.slot = alloca i8*, align 8
  %ehselector.slot = alloca i32, align 4
  %call = call noalias noundef nonnull i8* @_Znwm(i64 noundef 24) #5
  %0 = bitcast i8* %call to %class.MyVector*
  invoke void @_ZN8MyVectorC2Eii(%class.MyVector* noundef nonnull align 8 dereferenceable(20) %0, i32 noundef 10, i32 noundef 3)
          to label %invoke.cont unwind label %lpad

invoke.cont:                                      ; preds = %entry
  store %class.MyVector* %0, %class.MyVector** %v3, align 8
  ret i32 0

lpad:                                             ; preds = %entry
  %1 = landingpad { i8*, i32 }
          cleanup
  %2 = extractvalue { i8*, i32 } %1, 0
  store i8* %2, i8** %exn.slot, align 8
  %3 = extractvalue { i8*, i32 } %1, 1
  store i32 %3, i32* %ehselector.slot, align 4
  call void @_ZdlPv(i8* noundef %call) #6
  br label %eh.resume

eh.resume:                                        ; preds = %lpad
  %exn = load i8*, i8** %exn.slot, align 8
  %sel = load i32, i32* %ehselector.slot, align 4
  %lpad.val = insertvalue { i8*, i32 } undef, i8* %exn, 0
  %lpad.val1 = insertvalue { i8*, i32 } %lpad.val, i32 %sel, 1
  resume { i8*, i32 } %lpad.val1
}

; Function Attrs: nobuiltin allocsize(0)
declare noundef nonnull i8* @_Znwm(i64 noundef) #1

; Function Attrs: noinline optnone uwtable
define linkonce_odr dso_local void @_ZN8MyVectorC2Eii(%class.MyVector* noundef nonnull align 8 dereferenceable(20) %this, i32 noundef %sz, i32 noundef %val) unnamed_addr #2 comdat align 2 {
entry:
  %this.addr = alloca %class.MyVector*, align 8
  %sz.addr = alloca i32, align 4
  %val.addr = alloca i32, align 4
  store %class.MyVector* %this, %class.MyVector** %this.addr, align 8
  store i32 %sz, i32* %sz.addr, align 4
  store i32 %val, i32* %val.addr, align 4
  %this1 = load %class.MyVector*, %class.MyVector** %this.addr, align 8
  %obj = getelementptr inbounds %class.MyVector, %class.MyVector* %this1, i32 0, i32 2
  %0 = load i32, i32* %sz.addr, align 4
  %conv = sext i32 %0 to i64
  %1 = call { i64, i1 } @llvm.umul.with.overflow.i64(i64 %conv, i64 4)
  %2 = extractvalue { i64, i1 } %1, 1
  %3 = extractvalue { i64, i1 } %1, 0
  %4 = select i1 %2, i64 -1, i64 %3
  %call = call noalias noundef nonnull i8* @_Znam(i64 noundef %4) #5
  %5 = bitcast i8* %call to i32*
  %some_int = getelementptr inbounds %class.MyVector, %class.MyVector* %this1, i32 0, i32 0
  store i32* %5, i32** %some_int, align 8
  %6 = load i32, i32* %sz.addr, align 4
  %conv2 = sext i32 %6 to i64
  %7 = call { i64, i1 } @llvm.umul.with.overflow.i64(i64 %conv2, i64 4)
  %8 = extractvalue { i64, i1 } %7, 1
  %9 = extractvalue { i64, i1 } %7, 0
  %10 = select i1 %8, i64 -1, i64 %9
  %call3 = call noalias noundef nonnull i8* @_Znam(i64 noundef %10) #5
  %11 = bitcast i8* %call3 to %class.SomeClass*
  %some_class = getelementptr inbounds %class.MyVector, %class.MyVector* %this1, i32 0, i32 1
  store %class.SomeClass* %11, %class.SomeClass** %some_class, align 8
  %obj4 = getelementptr inbounds %class.MyVector, %class.MyVector* %this1, i32 0, i32 2
  %some_class5 = getelementptr inbounds %class.MyVector, %class.MyVector* %this1, i32 0, i32 1
  store %class.SomeClass* %obj4, %class.SomeClass** %some_class5, align 8
  ret void
}

declare i32 @__gxx_personality_v0(...)

; Function Attrs: nobuiltin nounwind
declare void @_ZdlPv(i8* noundef) #3

; Function Attrs: nofree nosync nounwind readnone speculatable willreturn
declare { i64, i1 } @llvm.umul.with.overflow.i64(i64, i64) #4

; Function Attrs: nobuiltin allocsize(0)
declare noundef nonnull i8* @_Znam(i64 noundef) #1

attributes #0 = { mustprogress noinline norecurse optnone uwtable "frame-pointer"="all" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #1 = { nobuiltin allocsize(0) "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #2 = { noinline optnone uwtable "frame-pointer"="all" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #3 = { nobuiltin nounwind "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #4 = { nofree nosync nounwind readnone speculatable willreturn }
attributes #5 = { builtin allocsize(0) }
attributes #6 = { builtin nounwind }

!llvm.module.flags = !{!0, !1, !2, !3, !4}
!llvm.ident = !{!5}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 7, !"PIC Level", i32 2}
!2 = !{i32 7, !"PIE Level", i32 2}
!3 = !{i32 7, !"uwtable", i32 1}
!4 = !{i32 7, !"frame-pointer", i32 2}
!5 = !{!"Ubuntu clang version 14.0.0-1ubuntu1.1"}
