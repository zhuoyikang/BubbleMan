msgbin:
	./proto_t.awk  proto.txt
	g++ -o msgbin MsgTest.cpp MsgBin.cpp MsgGen.cpp  ; ./msgbin

socknet:
	cp /Users/zhuoyikang/bubble_go/src/bubble/*.txt ./
	./proto_t.awk  proto.txt
	g++ -DSELF_BUBBLE_MAIN -o BubbleApp BubbleApp.cpp SockNet.cpp MsgBin.cpp MsgGen.cpp ; ./BubbleApp

run:
	./SockNet

g:
	./proto_t.awk  proto.txt

.PHONY:msgbin socknet g
