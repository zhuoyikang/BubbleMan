# 获取到所有的数据
# 加载proto文件到内存中使用.

# 打印参数
function PrintArgs()
{
    for(i=0;i<ARGC;i++) {
        print i,ARGV[i]
    }
}


function FindType(Type) {
    if (TypeMap[Type] != "") {
        return TypeMap[Type]
    }else {
        return Type
    }
}


# 加载payload.txt
function LoadPayLoad() {
    while( getline line < ARGV[1] ) { #由指定的文件中读取测验数据
        if (line ~ /^#.*/ || line ~ /^\s*$/ || line == "===" ) {
            continue
        }
        if(match(line,/^[^=].+=/) > 0 ) {
            name=substr(line,0,length(line)-1)
            PayLoadList[name,"count"] = 0
            PayLoadNames[PayLoadCount] = name
            PayLoadCount+=1
        } else {
            split(line,a," ")
            if (a[2] == "array") {
                fc = PayLoadList[name,"count"]+1
                PayLoadList[name,fc,"name"]=a[1]
                PayLoadList[name,fc,"type"]="array"
                PayLoadList[name,fc,"addtion"]=FindType(a[3])
                PayLoadList[name,"count"]= fc
            } else {
                fc = PayLoadList[name,"count"]+1
                PayLoadList[name,fc,"name"]=a[1]
                PayLoadList[name,fc,"type"]=FindType(a[2])
                PayLoadList[name,"count"]= fc
            }
        }
    }
}
