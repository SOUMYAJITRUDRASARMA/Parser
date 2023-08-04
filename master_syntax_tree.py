from igraph import Graph, EdgeSeq
import plotly.graph_objects as go

treeStr = '[ + [ + [ + [ + [ + [ + [ + [ + [ + [ + [ + [ + [ + [ + [ + [ + [ + [ + [ + [ + [ + [ + [ + [ + [ + [ . [ . [ . [ . [ . [ . [ D ] [ E ] ] [ L ] ] [ E ] ] [ T ] ] [ E ] ] [ # ] ] [ . [ . [ . [ . [ I ] [ N ] ] [ T ] ] [ O ] ] [ # ] ] ] [ . [ . [ . [ . [ . [ . [ I ] [ N ] ] [ S ] ] [ E ] ] [ R ] ] [ T ] ] [ # ] ] ] [ . [ . [ > ] [ = ] ] [ # ] ] ] [ . [ > ] [ # ] ] ] [ . [ = ] [ # ] ] ] [ . [ . [ < ] [ = ] ] [ # ] ] ] [ . [ < ] [ # ] ] ] [ . [ ) ] [ # ] ] ] [ . [ ( ] [ # ] ] ] [ . [ . [ . [ . [ . [ . [ V ] [ A ] ] [ L ] ] [ U ] ] [ E ] ] [ S ] ] [ # ] ] ] [ . [ . ] [ # ] ] ] [ . [ ; ] [ # ] ] ] [ . [ . [ B ] [ Y ] ] [ # ] ] ] [ . [ . [ O ] [ R ] ] [ # ] ] ] [ . [ . [ . [ . [ . [ . [ S ] [ E ] ] [ L ] ] [ E ] ] [ C ] ] [ T ] ] [ # ] ] ] [ . [ . [ . [ . [ . [ W ] [ H ] ] [ E ] ] [ R ] ] [ E ] ] [ # ] ] ] [ . [ . [ . [ . [ . [ G ] [ R ] ] [ O ] ] [ U ] ] [ P ] ] [ # ] ] ] [ . [ . [ . [ . [ . [ O ] [ R ] ] [ D ] ] [ E ] ] [ R ] ] [ # ] ] ] [ . [ . [ . [ . [ . [ . [ H ] [ A ] ] [ V ] ] [ I ] ] [ N ] ] [ G ] ] [ # ] ] ] [ . [ . [ . [ A ] [ N ] ] [ D ] ] [ # ] ] ] [ . [ . [ . [ . [ F ] [ R ] ] [ O ] ] [ M ] ] [ # ] ] ] [ . [ , ] [ # ] ] ] [ . [ . [ . [ N ] [ O ] ] [ T ] ] [ # ] ] ] [ . [ + [ + [ \n ] [ \t ] ] [   ] ] [ # ] ] ] [ . [ . [ + [ + [ + [ + [ + [ + [ + [ + [ + [ + [ + [ + [ + [ + [ + [ + [ + [ + [ + [ + [ + [ + [ + [ + [ + [ a ] [ b ] ] [ c ] ] [ d ] ] [ e ] ] [ f ] ] [ g ] ] [ h ] ] [ i ] ] [ j ] ] [ k ] ] [ l ] ] [ m ] ] [ n ] ] [ o ] ] [ p ] ] [ q ] ] [ r ] ] [ s ] ] [ t ] ] [ u ] ] [ v ] ] [ w ] ] [ x ] ] [ y ] ] [ z ] ] [ * [ + [ + [ + [ + [ + [ + [ + [ + [ + [ + [ + [ + [ + [ + [ + [ + [ + [ + [ + [ + [ + [ + [ + [ + [ + [ + [ + [ + [ + [ + [ + [ + [ + [ + [ + [ 1 ] [ 2 ] ] [ 3 ] ] [ 4 ] ] [ 5 ] ] [ 6 ] ] [ 7 ] ] [ 8 ] ] [ 9 ] ] [ 0 ] ] [ a ] ] [ b ] ] [ c ] ] [ d ] ] [ e ] ] [ f ] ] [ g ] ] [ h ] ] [ i ] ] [ j ] ] [ k ] ] [ l ] ] [ m ] ] [ n ] ] [ o ] ] [ p ] ] [ q ] ] [ r ] ] [ s ] ] [ t ] ] [ u ] ] [ v ] ] [ w ] ] [ x ] ] [ y ] ] [ z ] ] ] ] [ # ] ] ]'

# def generateTreeFromPreorderTraversal():



nr_vertices = 5
v_label = list(map(str, range(nr_vertices)))


G = Graph.Tree(nr_vertices, 2) # 2 stands for children number
G.add_vertices(nr_vertices)
G.vs[0]["label"] = "A"
G.vs[1]["label"] = "B"
G.vs[2]["label"] = "C"
G.vs[3]["label"] = "D"
G.vs[4]["label"] = "E"

G.add_edge(0, 1)
G.add_edge(0, 2)
G.add_edge(0, 3)
G.add_edge(3, 4)


lay = G.layout('rt')

position = {k: lay[k] for k in range(nr_vertices)}
Y = [lay[k][1] for k in range(nr_vertices)]
M = max(Y)

es = EdgeSeq(G) # sequence of edges
E = [e.tuple for e in G.es] # list of edges

L = len(position)
Xn = [position[k][0] for k in range(L)]
Yn = [2*M-position[k][1] for k in range(L)]
Xe = []
Ye = []
for edge in E:
    Xe+=[position[edge[0]][0],position[edge[1]][0], None]
    Ye+=[2*M-position[edge[0]][1],2*M-position[edge[1]][1], None]

labels = v_label

fig = go.Figure()
fig.add_trace(go.Scatter(x=Xe,
                   y=Ye,
                   mode='lines',
                   line=dict(color='rgb(210,210,210)', width=1),
                   hoverinfo='none'
                   ))
fig.add_trace(go.Scatter(x=Xn,
                  y=Yn,
                  mode='markers',
                  name='bla',
                  marker=dict(symbol='circle-dot',
                                size=18,
                                color='#6175c1',    #'#DB4551',
                                line=dict(color='rgb(50,50,50)', width=1)
                                ),
                  text=labels,
                  hoverinfo='text',
                  opacity=0.8
                  ))

fig.write_image("images/fig1.svg")