import pandas as pd

def mex(nums):
    if not nums:
        return 0
    for i in range(len(nums)+1):
        if nums.count(i) < 1:
            return i
    return 0

def is2pow(n):
    if n == 1:
        return 0
    i = 1
    while 1 << i < n:
        i += 1
    if 1 << i == n:
        return i
    return -1

def TniiBBtR(n,i):
    if i==0 or n<2 or n < 2*i:
        return -1
    if i==1:
        return (n-2)^1
    if n==6 and i==3:
        return 3
    if n!=2 and n!=6 and n==2*i:
        return 2
    return 0

def TniiBRtR(n,i):
    if i==0 or n<2 or n < 2*i:
        return -1
    if i==1:
        return (n-2)^1
    if n==7 and i==3:
        return 2
    if n!=3 and n!=7 and n%4==3 and n==2*i+1:
        return 3
    return 1

def TniiBBtB(n,i):
    if i==0 or n<2 or n < 2*i:
        return -1
    if i==2 and (n%2==1 or n%4==2):
        return n-3
    if i==2 and n%4==0:
        return n-1
    if n==8 and i==4:
        return 3
    if n!=8 and n==2*i:
        return 2
    return 0
    
def TniiBRtB(n,i):
    if i==0 or n<2 or n < 2*i:
        return -1
    if i==2 and (n%2==1 or n%4==0):
        return n-3
    if i==2 and n%4==2:
        return n-1
    if n%4==3 and n==2*i+1:
        return 3
    return 1

def TniiBB(n,i):
    if i==0 or n<3 or n < 2*i+1:
        return -1
    if n==3:
        return 3
    return 1

def TniiBR(n,i):
    if i==0 or n<3 or n < 2*i+1:
        return -1
    if n==4:
        return 2
    return 0

def TniiBtB(n,i):
    if i==0 or n<3 or n < i+2:
        return -1
    if i == 1 and n%2==1:
        return n
    if i == 1 and n%2==0:
        return n-3
    if i == 2 and n%2==1:
        return 0
    if i == 2 and n%2==0:
        return 1
    if (i==5 and n==10) or (i==7 and n == 12):
        return 7
    if (i==5 and n==11) or (i==7 and n==13):
        return 8
    if (m:=is2pow(i+1))>=2 and n==3*(1<<(m-1)) and (i!=7 or n!=12):
        return n-i+1
    if (m:=is2pow(i+1))>=2 and n>=3*(1<<(m-1))+1 and n % (1<<m) == (1<<(m-1)) + 1 and (i!=7 or n!=13):
        return n-i+1
    if i>=3 and i%2==1 and n>=i+2 and n<=2*i-1 and (i!=7 or n!=12) and (i!=7 or n!=13):
        return n-i-1
    if i>=3 and i%2==1 and n==2*i+2:
        return n-i-1
    if i>=3 and i%2==1 and n>=2*i+3 and n%2==1:
        return n-i-1
    if i>=3 and i%2==1 and (n==2*i or n==2*i+1) and i!=3 and i!=5:
        return n-i+1
    if i>=3 and i%2==1 and n>=2*i+4 and n%2==0:
        return n-2*i-1
    if i==4 and n>=8 and n%2==0:
        return n-i-3
    if i>=4 and i%2==0 and n>=i+2 and n<=2*i-1:
        return n-i-1
    if i>=6 and i%2==0 and n>=2*i and n%2==0:
        return n-i-1
    if i>=4 and i%2==0 and n>=2*i+1 and n%2==1:
        return n-2*i-1  
    assert False

def TniiBtR(n,i):
    if i==0 or n<3 or n < i+2:
        return -1
    if i==1 and n%2==1:
        return 0
    if i==1 and n%2==0:
            return 1
    if i==2 and n%2==1:
        return n-5
    if i==2 and n%2==0:
        return n-2
    if i == 3 and n>=7 and n%2==1:
        return n-2*i-1
    if (i==6 and n==11) or (i==4 and n==7) or (i==4 and n==8) or (i==8 and n==13):
        return 6
    if i==6 and n==12:
        return 9
    if (m:=is2pow(i))>=4 and n == (3 * (1<<(m-1))) + 1:
        return n-i+2
    if i>=3 and i%2 == 1 and n >= i + 2 and n <= 2*i+1:
        return (n-i-1)^1
    if i>=3 and i%2 == 1 and n >= 2*i+2 and n % 2 == 0:
        return n-2*i-1
    if i>=3 and i%2 == 1 and n >= 2*i+3 and n % 2 == 1:
        return n-i-2
    if i>=4 and i%2 == 0 and n >= i + 2 and n <= 2*i-2 and (i!=8 or n!=13):
        return (n-i-1)^1
    if i>=4 and i%2 == 0 and (n == 2*i or n == 2*i+1) and (i!=4 or n!=8) and (i!=6 or n!=12):
        return n-i
    if i>=4 and i%2 == 0 and n == 2*i-1 and (i!=4 or n!=7) and (i!=6 or n!=11):
        return n-i+2
    if i>=4 and i%2 == 0 and n >= 2*i+2 and n % 2 == 0:
        return n-i-2
    if i>=4 and i%2 == 0 and n >= 2*i+3 and n % 2 == 1:
        return n-2*i-1
    assert False

def TniitBtB(n,i): # equivalent to TnitB(n, i), 1 means non-zero
    if i==0 or n<3 or n < 2*i+1:
        return -1
    if n == 2*i+2 and n>= 12 and i%2==1:
        return 0
    return 1

def TniiB(n,i):
    if i==0 or n<4 or n < i+3:
        return -1
    if i==1 and n%2==0:
        return n-2
    if i==1 and n%2==1:
        return n-5
    if n==8 and i==4:
        return 7
    if n==9 and i==5:
        return 6
    return n-2

def TniitB(n,i): # 1 means non-zero
    if n%2==0:
        return 1
    if n>=3*i+4 and (n!=2*i+9 or n!=i+13) and (n!=2*i+9 or n!= i+14):
        return 1
    if is2pow(i)>=1 and n==4*i+3:
        return 1
    if is2pow(i)>=3 and n==4*i+5:
        return 1
    if (m:=is2pow(n-2*i-3))>=2 and i>=pow(2,m-1)+1 and i%pow(2,m)==pow(2,m-1)+1:
        return 1
    if i%2==0 and n==2*i+3:
        return 1
    if i%2==0 and i>=4 and (n==3*i+1 or n==3*i+3):
        return 1
    if i%2==0 and i>=6 and n==3*i+5:
        return 1
    if i%2==0 and n>=3*i+6:
        return 1
    if i%2==1 and i>=5 and n==2*i+3:
        return 1
    if i%2==1 and n==2*i+5:
        return 1
    if i%2==1 and n>=2*i+5 and n<=3*i:
        return 1
    if i%2==1 and n>=3*i+4:
        return 1
    return 0

def Tnii(n,i): # 1 means non-zero
    if i==0 or n<5 or n < 2*i + 3:
        return -1
    if i%2==1 and (n==2*i+4 or n==2*i+5):
        return 1
    if i==2 and n==16:
        return 1
    if n>=2*i+7 and n%2==1 and (n!=15 or n!=2*i+11) and (n!=17 or n!=2*i+13):
        return 1
    if n==2*i+3:
        return 1
    if TniitB(n-1,i)==0:
        return 1
    return 0

def judge(filename, expected, nonzerojudge):
    df = pd.read_csv(filename,header=None)
    print(expected.__name__,":")
    count = 0
    for i in range(df.shape[0]):
        for n in range(df.shape[0]):
            result = expected(n,i)
            if not ((result == df[i][n]) or (nonzerojudge and result > 0 and df[i][n] > 0)) and df[i][n]!=-1:
                print("\tn = ",n,", i = ",i,", expected:",result,", value:",df[i][n])
                count += 1
    if count == 0:
        print("\tNo wrong")
    print("")
    return

def main():
    judge("result/Tnii.csv",Tnii,True)
    judge("result/TniiB.csv",TniiB,False)
    judge("result/TniitB.csv",TniitB,True)
    judge("result/TniiBtB.csv",TniiBtB,False)
    judge("result/TniiBtR.csv",TniiBtR,False)
    judge("result/TniiBB.csv",TniiBB,False)
    judge("result/TniiBR.csv",TniiBR,False)
    judge("result/TniitBtB.csv",TniitBtB,True)
    judge("result/TniiBBtR.csv",TniiBBtR,False)
    judge("result/TniiBRtR.csv",TniiBRtR,False)
    judge("result/TniiBBtB.csv",TniiBBtB,False)
    judge("result/TniiBRtB.csv",TniiBRtB,False)
    return

if __name__ == "__main__":
    main()
