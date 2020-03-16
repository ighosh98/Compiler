with open("productions.txt","r") as f:
    with open("ans.txt","w") as fout:
        for i in f:
            x = i.split();
            a = x[0] + "  ->   "
            for j in x[1:]:
                a = a + j + "   "
            a = a+'\n'
            fout.write(a);

