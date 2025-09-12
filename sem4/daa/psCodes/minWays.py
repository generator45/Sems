def minWays(s):
    for i in range( 0, len(s) ):
        min = 0
        for j in range( 0, i+1):
            if(isWord(s[j:i+1])):
                min = dp[j]