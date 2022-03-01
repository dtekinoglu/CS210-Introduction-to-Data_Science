# Püren Tap
# Dilara Nur Tekinoğlu

import multiprocessing
from multiprocessing import Process, Semaphore, Lock , Barrier
import random as rnd
import zmq
import os
import sys


# This function returns CRASH with probability "prob", else it returns "SUCCESS"
def getAnswer(prob):
    answers = ["CRASH", "SUCCESS"]
    distribution = [prob, 1 - prob]
    answer = rnd.choices(answers, distribution)
    return answer


def broadcastFailure(socket_list, msg, proposer, N, prob):
    for i in range(N):
        socket = socket_list[i]
        sendFailure(socket, msg, proposer, i, prob)


def sendFailure(push_socket, msg, proposer, i, prob):

    ans = getAnswer(prob)
    if ans[0] == "CRASH":
        msg = "CRASH "  + str(proposer)
    push_socket.send_string(msg)


def PaxosNode(barrier_round,print_mut, ID, prob, N, val, numRounds):
    # initialization of local variables
    maxVotedRound = -1
    maxVotedVal = None
    proposedVal = val  
    decision = None
    context = zmq.Context()
    context2 = zmq.Context()
    host = "127.0.0.1"
    port = 5550 #base port
    pushSocketList =[]

    #initializing N Push sockets for each node
    for i in range(N):
        pushSocket = context.socket(zmq.PUSH)
        port = str(5550 + i)
        pushSocket.connect("tcp://" + host + ":" + port)

        pushSocketList.append(pushSocket)

    #initializing a pull socket for the current node

    pullSocket = context2.socket(zmq.PULL)
    port = str(5550 + ID)
    pullSocket.bind("tcp://" + host + ":" + port)

    for round in range(numRounds):

        isLeader = round % N == ID

        # if the node is the leader of the current round.
        if isLeader == False:
            # block until receive a message from the leader
            ans = pullSocket.recv_string()

            print_mut.acquire()
            print("ACCEPTOR ", ID, " RECEIVED IN JOIN PHASE: ", ans)
            print_mut.release()

            ans = ans.split()

            # if the leader crashes, send the following message
            if ans[0] == "CRASH":

                nodeNum = int(ans[1]) #proposer of that round
                socketToSend = pushSocketList[nodeNum]
                msg = "CRASH " + str(round % N)
                socketToSend.send_string(msg)

            # if it receives START msg from the leader, do the following
            else:
                proposerSocket = pushSocketList[round%N]
                msg = "JOIN " + str(maxVotedRound) + " " + str(maxVotedVal)
                sendFailure(proposerSocket, msg, round % N, ID, prob)

            ans2 = pullSocket.recv_string()
            print_mut.acquire()
            print("ACCEPTOR ", ID, " RECEIVED IN VOTE PHASE: ", ans2)
            print_mut.release()

            ans2 = ans2.split(" ")

            if ans2[0] == "CRASH": # This means the leader is crashed in vote phase.
                nodeNum = int(ans2[1]) #proposer of that round
                proposerSocket = pushSocketList[nodeNum]
                msg = "CRASH " + str(round % N)
                proposerSocket.send_string(msg)
                barrier_round.wait()

            elif ans2[0] == "ROUNDCHANGE":
                barrier_round.wait()
                continue  # Direcly go to the next round

            elif ans2[0] == "PROPOSE":
                maxVotedVal = ans2[1]
                maxVotedRound = round
                msg = "VOTE"
                proposerSocket = pushSocketList[round%N]
                sendFailure(proposerSocket, msg, round % N, ID, prob)
                barrier_round.wait()


        # if the node is the leader of the round
        else:
            print_mut.acquire()
            print("ROUND ", round, " HAS STARTED WITH INITIAL VALUE ", proposedVal)
            print_mut.release()


            broadcastFailure(pushSocketList, "START", round % N, N, prob)  # it first broadcast start message to all nodes including itself.

            maxRoundJoined = -1
            maxValJoined = None

            countJoin = 0
            countVote = 0
            didPropose = False
            receivedMsg = 0

            while receivedMsg < N:
                ans = pullSocket.recv_string()
                receivedMsg += 1

                print_mut.acquire()
                print("LEADER OF ", round ," RECEIVED IN JOIN PHASE: ", ans)
                print_mut.release()

                ans = ans.split(" ")
                if ans[0] == "START":
                    countJoin += 1
                    receivedStart = True #proposer sent itself a START message hence it did not crash as an acceptor

                elif ans[0] == "JOIN":
                    countJoin += 1
                    currentRound = int(ans[1])

                    if currentRound > maxRoundJoined:
                        maxRoundJoined = currentRound
                        if ans[2] != None : 
                            maxValJoined = int(ans[2])

            if countJoin > N / 2:
                if maxVotedRound > maxRoundJoined: #check the maxVotedRound of current node
                    proposedVal = maxVotedVal
                elif maxRoundJoined == -1: #none of the nodes in join quorum has voted before
                    proposedVal = val
                else:
                    proposedVal = maxValJoined


                didPropose = True
                msg = "PROPOSE " + str(proposedVal)
                broadcastFailure(pushSocketList, msg, round % N, N, prob)

            elif countJoin <= N/2:
                #broadcast ROUNDCHANGE to every other node except itself
                print_mut.acquire()
                print("LEADER OF ", round, "CHANGED ROUND ")
                print_mut.release()

                for nodeID in range(N):
                    if nodeID != ID :
                        push_socket = pushSocketList[nodeID]
                        push_socket.send_string("ROUNDCHANGE")
                barrier_round.wait()

            #leader has proposed a value and now it has to wait for N messages
            receivedMsg =0
            if didPropose:
                while receivedMsg < N:
                    ans = pullSocket.recv_string()
                    receivedMsg += 1
                    print_mut.acquire()
                    print("LEADER OF ", round, "RECEIVED IN VOTE PHASE ", ans)
                    print_mut.release()

                    ans = ans.split(" ")

                    if ans[0] == "PROPOSE": #proposer receives its own propose message
                        countVote += 1
                        maxVotedRound = round
                        maxVotedVal = proposedVal
                        receivedPropose = True

                    elif ans[0] == "VOTE":
                        countVote += 1

                    else: #proposer receives CRASH
                        pass


                if countVote >  N/2: #proposer comes to a decision
                    decision = proposedVal
                    print_mut.acquire()
                    print("LEADER OF ", round, "DECIDED ON VALUE ", decision)
                    print_mut.release()
                barrier_round.wait()



if __name__ == "__main__":
    '''
    numProc = 5
    prob = 0.4
    numRounds = 10
    '''


    numProc = int(sys.argv[1])
    prob = float(sys.argv[2])
    numRounds = int(sys.argv[3])

    barrier_round = multiprocessing.Barrier(numProc)
    print_mut = Lock()  # mutex for printing

    print("NUM_NODES: ", numProc, ", CRASH_PROB: ", prob , "NUM_ROUNDS: ", numRounds)

    # Creating Paxos nodes
    nodeList = []
    for i in range(numProc):
        val = rnd.randint(0, 1)
        node = Process(target=PaxosNode, args=(barrier_round, print_mut, i, prob, numProc, val, numRounds))
        nodeList.append(node)

    # start nodes
    for i in range(numProc):
        nodeList[i].start()

    # join nodes
    for i in range(numProc):
        nodeList[i].join()
