#include <JuceHeader.h>
#include <iostream>
#include <vector>
#include <queue>
#include <limits>
#include <stack>
#include <random>
#include <algorithm>

struct Node 
{
    int vertex;
    std::vector<int> path;
    int bound;
    int level;
};

int calculateBound(const std::vector<std::vector<int>>& adjacencyMatrix, const Node& node)
{
    int bound = node.bound;
    for (int i = 0; i < adjacencyMatrix.size(); ++i) {
        if (adjacencyMatrix[node.vertex][i] != 0 && std::find(node.path.begin(), node.path.end(), i) == node.path.end()) 
        {
            int minValue = std::numeric_limits<int>::max();
            for (int j = 0; j < adjacencyMatrix.size(); ++j)
                if (j != i && adjacencyMatrix[i][j] != 0 && std::find(node.path.begin(), node.path.end(), j) == node.path.end()) minValue = std::min(minValue, adjacencyMatrix[i][j]);
            bound += minValue;
        }
    } return bound;
}

std::vector<int> solveTSP(const std::vector<std::vector<int>>& adjacencyMatrix) {
    int n = adjacencyMatrix.size();
    Node rootNode;
    rootNode.vertex = 0;
    rootNode.bound = 0;
    rootNode.level = 0;
    rootNode.path.push_back(0);
    std::vector<Node> nodeQueue;
    nodeQueue.push_back(rootNode);
    int minCost = std::numeric_limits<int>::max();
    std::vector<int> minPath;
    while (!nodeQueue.empty()) 
    {
        Node currentNode = nodeQueue.front();
        nodeQueue.erase(nodeQueue.begin());
        if (currentNode.level == n - 1)
        {
            int cost = currentNode.bound + adjacencyMatrix[currentNode.vertex][0];
            if (cost < minCost) 
            {
                minCost = cost;
                minPath = currentNode.path;
                minPath.push_back(0);
            }
        }
        for (int i = 0; i < n; ++i) {
            if (adjacencyMatrix[currentNode.vertex][i] != 0 && std::find(currentNode.path.begin(), currentNode.path.end(), i) == currentNode.path.end())
            {
                Node newNode;
                newNode.vertex = i;
                newNode.path = currentNode.path;
                newNode.path.push_back(i);
                newNode.bound = currentNode.bound + adjacencyMatrix[currentNode.vertex][i];
                newNode.level = currentNode.level + 1;
                if (newNode.bound < minCost) 
                {
                    newNode.bound = calculateBound(adjacencyMatrix, newNode);
                    nodeQueue.push_back(newNode);
                }
            }
        }
    }

    return minPath;
}

class Graph_logics
{
    std::vector<std::vector<int>> adjacencyMatrix;
public:
    Graph_logics() { adjacencyMatrix = std::vector<std::vector<int>>(); }
    void addVertex();
    void removeVertex(int vertex);
    void addEdge(int source, int destination, int weight);
    void setEdgeWeight(int source, int destination, int weight);
    void removeEdge(int source, int destination);
    void printAdjacencyMatrix();
    auto getAdjacencyMatrix();
    void setAdjacencyMatrixValue(int source, int destination, int value);
    std::vector<int> breadthFirstTraversal(int startVertex);
    std::vector<int> depthFirstTraversal(int startVertex);
    std::vector<std::pair<int, int>> dijkstraAlgorithm(int startVertex);
};

void Graph_logics::addVertex()
{
    int numVertices = adjacencyMatrix.size();
    for (int i = 0; i < numVertices; i++) adjacencyMatrix[i].push_back(0);
    adjacencyMatrix.push_back(std::vector<int>(numVertices + 1, 0));
}

void Graph_logics::removeVertex(int vertex)
{
    int numVertices = adjacencyMatrix.size();
    if (vertex >= numVertices) { return; }
    adjacencyMatrix.erase(adjacencyMatrix.begin() + vertex);
    for (auto& row : adjacencyMatrix) row.erase(row.begin() + vertex);
}

void Graph_logics::addEdge(int source, int destination, int weight)
{
    int numVertices = adjacencyMatrix.size();
    if (source >= numVertices || destination >= numVertices) { std::cout << "Invalid vertices!" << std::endl; return; }
    adjacencyMatrix[source][destination] = weight;
}

void Graph_logics::setEdgeWeight(int source, int destination, int weight)
{
    if (source >= 0 && source < adjacencyMatrix.size() &&
        destination >= 0 && destination < adjacencyMatrix.size())
    {
        adjacencyMatrix[source][destination] = weight;
        adjacencyMatrix[destination][source] = weight;
    }
    else {}
}

void Graph_logics::removeEdge(int source, int destination)
{
    int numVertices = adjacencyMatrix.size();
    if (source >= numVertices || destination >= numVertices) { std::cout << "Invalid vertices!" << std::endl; return; }
    adjacencyMatrix[source][destination] = 0;
}

void Graph_logics::printAdjacencyMatrix()
{
    int numVertices = adjacencyMatrix.size();
    for (int i = 0; i < numVertices; i++)
    {
        for (int j = 0; j < numVertices; j++) std::cout << adjacencyMatrix[i][j] << " ";
        std::cout << std::endl;
    }
}

auto Graph_logics::getAdjacencyMatrix() { return adjacencyMatrix; }

void Graph_logics::setAdjacencyMatrixValue(int source, int destination, int value) 
{ 
    if (adjacencyMatrix[source][destination] == 0) { return; }
    adjacencyMatrix[source][destination] = value; 
    if (adjacencyMatrix[destination][source] > 0) { adjacencyMatrix[destination][source] = value;}
}

std::vector<int> Graph_logics::breadthFirstTraversal(int startVertex)
{
    std::vector<int> result;
    std::vector<bool> visited(adjacencyMatrix.size(), false);
    std::queue<int> queue;
    queue.push(startVertex);
    visited[startVertex] = true;
    while (!queue.empty())
    {
        int currentVertex = queue.front();
        queue.pop();
        result.push_back(currentVertex);
        for (int i = 0; i < adjacencyMatrix[currentVertex].size(); ++i)
            if (adjacencyMatrix[currentVertex][i] > 0 && !visited[i])
            {
                queue.push(i);
                visited[i] = true;
            }
    }

    return result;
}

std::vector<int> Graph_logics::depthFirstTraversal(int startVertex)
{
    std::vector<int> result;
    std::vector<bool> visited(adjacencyMatrix.size(), false);
    std::stack<int> stack;
    stack.push(startVertex);
    visited[startVertex] = true;
    while (!stack.empty())
    {
        int currentVertex = stack.top();
        stack.pop();
        result.push_back(currentVertex);
        for (int i = 0; i < adjacencyMatrix[currentVertex].size(); ++i)
        {
            if (adjacencyMatrix[currentVertex][i] > 0 && !visited[i])
            {
                stack.push(i);
                visited[i] = true;
            }
        }
    } return result;
}

std::vector<std::pair<int, int>> Graph_logics::dijkstraAlgorithm(int startVertex)
{
    int SIZE = adjacencyMatrix.size();
    std::vector<int> d(SIZE, std::numeric_limits<int>::max());
    std::vector<int> v(SIZE, 1);
    d[startVertex] = 0;
    int minindex;
    do
    {
        minindex = std::numeric_limits<int>::max();
        int min = std::numeric_limits<int>::max();
        for (int i = 0; i < SIZE; i++)
        {
            if (v[i] == 1 && d[i] < min)
            {
                min = d[i];
                minindex = i;
            }
        }
        if (minindex != std::numeric_limits<int>::max())
        {
            for (int i = 0; i < SIZE; i++)
            {
                if (adjacencyMatrix[minindex][i] > 0)
                {
                    int temp = min + adjacencyMatrix[minindex][i];
                    if (temp < d[i]) d[i] = temp;
                }
            } v[minindex] = 0;
        }
    } while (minindex < std::numeric_limits<int>::max());
    std::vector<std::pair<int, int>> result;
    for (int i = 0; i < SIZE; i++) result.push_back(std::make_pair(i, d[i]));
    return result;
}

class GraphComponent : public juce::Component, public juce::Button::Listener, public juce::MouseListener, public juce::TextEditor::Listener
{
public:
    GraphComponent()
    {
        addAndMakeVisible(addButton);
        addButton.setButtonText("Add Vertex");
        addButton.addListener(this);
        addButton.setColour(juce::TextButton::buttonColourId, juce::Colours::green);
        addAndMakeVisible(removeButton);
        removeButton.setButtonText("Remove Vertex");
        removeButton.addListener(this);
        removeButton.setColour(juce::TextButton::buttonColourId, juce::Colours::red);
        addAndMakeVisible(addEdgeButton);
        addEdgeButton.setButtonText("Add Edge");
        addEdgeButton.addListener(this);
        addEdgeButton.setColour(juce::TextButton::buttonColourId, juce::Colours::brown);
        addAndMakeVisible(removeEdgeButton);
        removeEdgeButton.setButtonText("Remove Edge");
        removeEdgeButton.addListener(this);
        removeEdgeButton.setColour(juce::TextButton::buttonColourId, juce::Colours::grey);
        addAndMakeVisible(weightButton);
        weightButton.setButtonText("Set Weight");
        weightButton.addListener(this);
        weightButton.setColour(juce::TextButton::buttonColourId, juce::Colours::coral);
        addAndMakeVisible(weightInput);
        weightInput.setColour(juce::TextEditor::textColourId, juce::Colours::black);
        weightInput.setColour(juce::TextEditor::backgroundColourId, juce::Colours::cadetblue);
        weightInput.addListener(this);
        weightInput.setVisible(false);
        addAndMakeVisible(DFS);
        DFS.setButtonText("DFS");
        DFS.addListener(this);
        DFS.setColour(juce::TextButton::buttonColourId, juce::Colours::burlywood);
        addAndMakeVisible(BFS);
        BFS.setButtonText("BFS");
        BFS.addListener(this);
        BFS.setColour(juce::TextButton::buttonColourId, juce::Colours::chartreuse);
        addAndMakeVisible(Dji);
        Dji.setButtonText("SW");
        Dji.addListener(this);
        Dji.setColour(juce::TextButton::buttonColourId, juce::Colours::chocolate);
        addAndMakeVisible(TSP);
        TSP.setButtonText("TSP");
        TSP.addListener(this);
        TSP.setColour(juce::TextButton::buttonColourId, juce::Colours::crimson);
        addAndMakeVisible(algorimInput);
        algorimInput.setColour(juce::TextEditor::textColourId, juce::Colours::black);
        algorimInput.setColour(juce::TextEditor::backgroundColourId, juce::Colours::cornsilk);
        algorimInput.addListener(this);
        algorimInput.setVisible(false);
    }

    void addEdge(int source, int destination, int weight)
    {
        graph.addEdge(sourceVertex, destinationVertex, weight); repaint();
    }

    void removeEdge(int source, int destination)
    {
        graph.setAdjacencyMatrixValue(source, destination, 0);
        graph.setAdjacencyMatrixValue(destination, source, 0);
        repaint();
        if (isRemovingEdge) { isRemovingEdge = false; removeEdgeButton.setButtonText("Remove Edge"); }
    }

    void paint(juce::Graphics& g) override
    {
        g.fillAll(juce::Colours::white);
        const int radius = 200;
        const juce::Point<int> center(getWidth() / 2, getHeight() / 2);
        juce::Font font(20, juce::Font::bold);
        g.setFont(font);
        const int numVertices = graph.getAdjacencyMatrix().size();
        const float angleStep = 2 * juce::MathConstants<float>::pi / numVertices;
        const std::vector<std::vector<int>>& adjacencyMatrix = graph.getAdjacencyMatrix();
        for (int i = 0; i < numVertices; ++i)
        {
            for (int j = 0; j < numVertices; ++j)
            {
                int weight = adjacencyMatrix[i][j];
                if (weight > 0)
                {
                    const float angle1 = i * angleStep;
                    const float angle2 = j * angleStep;
                    const int x1 = static_cast<int>(center.x + radius * std::cos(angle1));
                    const int y1 = static_cast<int>(center.y + radius * std::sin(angle1));
                    const int x2 = static_cast<int>(center.x + radius * std::cos(angle2));
                    const int y2 = static_cast<int>(center.y + radius * std::sin(angle2));
                    bool isDirected = (weight != adjacencyMatrix[j][i]);
                    juce::Path path;
                    path.startNewSubPath(x1, y1);
                    if (isDirected)
                    {
                        float angle = std::atan2(y1 - y2, x1 - x2);
                        float arrowSize = 10.0f;
                        float arrowAngle = juce::MathConstants<float>::pi * 0.15f;
                        float touchX = x2 + (vertexSize / 2.0f - 1.0f) * std::cos(angle);
                        float touchY = y2 + (vertexSize / 2.0f - 1.0f) * std::sin(angle);
                        juce::Point<float> arrow1(touchX + arrowSize * std::cos(angle - arrowAngle),
                            touchY + arrowSize * std::sin(angle - arrowAngle));
                        juce::Point<float> arrow2(touchX + arrowSize * std::cos(angle + arrowAngle),
                            touchY + arrowSize * std::sin(angle + arrowAngle));
                        path.lineTo(touchX, touchY);
                        path.lineTo(arrow1);
                        path.startNewSubPath(touchX, touchY);
                        path.lineTo(arrow2);
                    }
                    else
                    {
                        float angle = std::atan2(y2 - y1, x2 - x1);
                        float touchX1 = x1 + (vertexSize / 2.0f - 1.0f) * std::cos(angle);
                        float touchY1 = y1 + (vertexSize / 2.0f - 1.0f) * std::sin(angle);
                        float touchX2 = x2 + (vertexSize / 2.0f - 1.0f) * std::cos(angle + juce::MathConstants<float>::pi);
                        float touchY2 = y2 + (vertexSize / 2.0f - 1.0f) * std::sin(angle + juce::MathConstants<float>::pi);
                        path.lineTo(touchX1, touchY1);
                        path.lineTo(touchX2, touchY2);
                    }
                    const int weightX = (x1 + x2) / 2;
                    const int weightY = (y1 + y2) / 2;
                    juce::String weightText = juce::String(weight);
                    g.drawText(weightText, weightX, weightY, 40, 20, juce::Justification::left);
                    g.strokePath(path, juce::PathStrokeType(1.0f));
                }
            }
        }

        // Отрисовка вершин
        for (int i = 0; i < numVertices; ++i)
        {
            const float angle = i * angleStep;
            const int x = static_cast<int>(center.x + radius * std::cos(angle)) - vertexSize / 2;
            const int y = static_cast<int>(center.y + radius * std::sin(angle)) - vertexSize / 2;
            juce::Rectangle<int> vertexBounds(x, y, vertexSize, vertexSize);
            g.setColour(vertexColors[i % vertexColors.size()]);
            g.fillEllipse(vertexBounds.toFloat());
            g.setColour(juce::Colours::black);
            g.drawEllipse(vertexBounds.toFloat(), 1);
            g.drawText(juce::String(i + 1), vertexBounds, juce::Justification::centred);
        }
    }

    void resized() override
    {
        addButton.setBounds(10, 10, 100, 30);
        removeButton.setBounds(10, 50, 100, 30);
        addEdgeButton.setBounds(10, 90, 100, 30);
        removeEdgeButton.setBounds(10, 130, 100, 30);
        weightButton.setBounds(10, 170, 100, 30);
        weightInput.setBounds(120, 170, 50, 30);
        DFS.setBounds(10, 210, 45, 30);
        BFS.setBounds(65, 210, 45, 30);
        Dji.setBounds(10, 250, 45, 30);
        TSP.setBounds(65, 250, 45, 30);
        algorimInput.setBounds(10, 290, 100, 30);
    }

    void buttonClicked(juce::Button* button) override
    {
        if (button == &addButton)
        {
            numberField.setVisible(false);
            if (graph.getAdjacencyMatrix().size() < maxVertices) { graph.addVertex(); repaint(); }
        }
        else if (button == &removeButton)
        {
            numberField.setVisible(false);
            DFS.setEnabled(false);
            BFS.setEnabled(false);
            Dji.setEnabled(false);
            TSP.setEnabled(false);
            addButton.setEnabled(false);
            addEdgeButton.setEnabled(false);
            weightButton.setEnabled(false);
            removeEdgeButton.setEnabled(false);
            if (!isRemovingVertex)
            {
                isRemovingVertex = true;
                removeButton.setButtonText("Cancel");
            }
            else
            {
                isRemovingVertex = false;
                removeButton.setButtonText("Remove Vertex");
                addButton.setEnabled(true);
                addEdgeButton.setEnabled(true);
                weightButton.setEnabled(true);
                removeEdgeButton.setEnabled(true);
                DFS.setEnabled(true);
                BFS.setEnabled(true);
                Dji.setEnabled(true);
                TSP.setEnabled(true);
            }
        }
        else if (button == &removeEdgeButton)
        {
            numberField.setVisible(false);
            DFS.setEnabled(false);
            BFS.setEnabled(false);
            Dji.setEnabled(false);
            TSP.setEnabled(false);
            addButton.setEnabled(false);
            addEdgeButton.setEnabled(false);
            weightButton.setEnabled(false);
            removeButton.setEnabled(false);
            if (!isRemovingEdge)
            {
                isRemovingEdge = true;
                removeEdgeButton.setButtonText("Cancel");
            }
            else
            {
                isRemovingEdge = false;
                removeEdgeButton.setButtonText("Remove Edge");
                addButton.setEnabled(true);
                addEdgeButton.setEnabled(true);
                weightButton.setEnabled(true);
                removeButton.setEnabled(true);
                DFS.setEnabled(true);
                BFS.setEnabled(true);
                Dji.setEnabled(true);
                TSP.setEnabled(true);
            }
        }
        else if (button == &addEdgeButton)
        {
            numberField.setVisible(false);
            DFS.setEnabled(false);
            BFS.setEnabled(false);
            Dji.setEnabled(false);
            TSP.setEnabled(false);
            addButton.setEnabled(false);
            removeEdgeButton.setEnabled(false);
            removeButton.setEnabled(false);
            weightButton.setEnabled(false);
            if (!isAddingEdge)
            {
                isAddingEdge = true;
                addEdgeButton.setButtonText("Cancel");
            }
            else
            {
                isAddingEdge = false;
                sourceVertex = -1;
                destinationVertex = -1;
                addEdgeButton.setButtonText("Add Edge");
                addButton.setEnabled(true);
                removeEdgeButton.setEnabled(true);
                removeButton.setEnabled(true);
                weightButton.setEnabled(true);
                DFS.setEnabled(true);
                BFS.setEnabled(true);
                Dji.setEnabled(true);
                TSP.setEnabled(true);
            }
        }
        else if (button == &weightButton)
        {
            numberField.setVisible(false);
            DFS.setEnabled(false);
            BFS.setEnabled(false);
            Dji.setEnabled(false);
            TSP.setEnabled(false);
            addButton.setEnabled(false);
            removeButton.setEnabled(false);
            addEdgeButton.setEnabled(false);
            removeEdgeButton.setEnabled(false);
            if (!isSettingWeight)
            {
                isSettingWeight = true;
                weightButton.setButtonText("Cancel");
            }
            else
            {
                isSettingWeight = false;
                weightButton.setButtonText("Set Weight");
                weightInput.setVisible(false);
                addButton.setEnabled(true);
                removeButton.setEnabled(true);
                addEdgeButton.setEnabled(true);
                removeEdgeButton.setEnabled(true);
                DFS.setEnabled(true);
                BFS.setEnabled(true);
                Dji.setEnabled(true);
                TSP.setEnabled(true);
            }
        }
        else if (button == &DFS)
        {
            numberField.setVisible(false);
            if (!dfs)
            {
                algorimInput.setVisible(true);
                algorimInput.grabKeyboardFocus();
                dfs = true;
            }
            else
            {
                dfs = false;
                algorimInput.setVisible(false);
                numberField.clear();
            }
        }
        else if (button == &BFS)
        {
            numberField.setVisible(false);
            if (!bfs)
            {
                algorimInput.setVisible(true);
                algorimInput.grabKeyboardFocus();
                dfs = true;
            }
            else
            {
                bfs = false;
                algorimInput.setVisible(false);
                numberField.clear();
            }
        }
        else if (button == &Dji)
        {
            numberField.setVisible(false);
            if (!dji)
            {
                algorimInput.setVisible(true);
                algorimInput.grabKeyboardFocus();
                dji = true;
            }
            else
            {
                dji = false;
                algorimInput.setVisible(false);
                numberField.clear();
            }
        } else if (button == &TSP)
        {
            auto input = solveTSP(graph.getAdjacencyMatrix());
            juce::String numberText;
            if (input.size() > 1) for (auto number : input) numberText += juce::String(number + 1) + " ";
            else numberText = "No solution found.";
            numberField.setBounds(10, 290, 100, 30);
            numberField.setColour(juce::TextEditor::textColourId, juce::Colours::white);
            numberField.setText(numberText);
            addAndMakeVisible(numberField);
            numberField.setVisible(true);
        }
    }

    void mouseUp(const juce::MouseEvent& event) override
    {
        if (isRemovingVertex)
        {
            const juce::Point<int> mousePos = event.getPosition();
            const int numVertices = graph.getAdjacencyMatrix().size();
            const int radius = 200;
            const juce::Point<int> center(getWidth() / 2, getHeight() / 2);
            const float angleStep = 2 * juce::MathConstants<float>::pi / numVertices;
            for (int i = 0; i < numVertices; ++i)
            {
                const float angle = i * angleStep;
                const int x = static_cast<int>(center.x + radius * std::cos(angle)) - vertexSize / 2;
                const int y = static_cast<int>(center.y + radius * std::sin(angle)) - vertexSize / 2;
                juce::Rectangle<int> vertexBounds(x, y, vertexSize, vertexSize);
                if (vertexBounds.contains(mousePos))
                {
                    graph.removeVertex(i);
                    repaint();
                    break;
                }
            }
            isRemovingVertex = false;
            removeButton.setButtonText("Remove Vertex");
            addButton.setEnabled(true);
            addEdgeButton.setEnabled(true);
            weightButton.setEnabled(true);
            removeEdgeButton.setEnabled(true);
            DFS.setEnabled(true);
            BFS.setEnabled(true);
            Dji.setEnabled(true);
            TSP.setEnabled(true);
        }
        else if (isAddingEdge)
        {
            const juce::Point<int> mousePos = event.getPosition();
            const int numVertices = graph.getAdjacencyMatrix().size();
            const int radius = 200;
            const juce::Point<int> center(getWidth() / 2, getHeight() / 2);
            const float angleStep = 2 * juce::MathConstants<float>::pi / numVertices;
            for (int i = 0; i < numVertices; ++i)
            {
                const float angle = i * angleStep;
                const int x = static_cast<int>(center.x + radius * std::cos(angle)) - vertexSize / 2;
                const int y = static_cast<int>(center.y + radius * std::sin(angle)) - vertexSize / 2;
                juce::Rectangle<int> vertexBounds(x, y, vertexSize, vertexSize);
                if (vertexBounds.contains(mousePos))
                {
                    if (sourceVertex == -1)
                    {
                        sourceVertex = i;
                        break;
                    }
                    else if (destinationVertex == -1 && i != sourceVertex)
                    {
                        destinationVertex = i;
                        isAddingEdge = false;
                        addEdgeButton.setButtonText("Add Edge");
                        graph.setAdjacencyMatrixValue(destinationVertex, sourceVertex, 1);
                        addEdge(sourceVertex, destinationVertex, 1);
                        sourceVertex = -1;
                        destinationVertex = -1;
                        addButton.setEnabled(true);
                        removeEdgeButton.setEnabled(true);
                        removeButton.setEnabled(true);
                        weightButton.setEnabled(true);
                        DFS.setEnabled(true);
                        BFS.setEnabled(true);
                        Dji.setEnabled(true);
                        TSP.setEnabled(true);
                    }
                }
            }
        }
        else if (isRemovingEdge)
        {
            const juce::Point<int> mousePos = event.getPosition();
            const int numVertices = graph.getAdjacencyMatrix().size();
            const int radius = 200;
            const juce::Point<int> center(getWidth() / 2, getHeight() / 2);
            const float angleStep = 2 * juce::MathConstants<float>::pi / numVertices;

            for (int i = 0; i < numVertices; ++i)
            {
                const float angle = i * angleStep;
                const int x = static_cast<int>(center.x + radius * std::cos(angle)) - vertexSize / 2;
                const int y = static_cast<int>(center.y + radius * std::sin(angle)) - vertexSize / 2;
                juce::Rectangle<int> vertexBounds(x, y, vertexSize, vertexSize);

                if (vertexBounds.contains(mousePos))
                {
                    if (removingSource == -1)
                    {
                        removingSource = i;
                        break;
                    }
                    else if (removingDestination == -1 && i != removingSource)
                    {
                        removingDestination = i;
                        removeEdge(removingSource, removingDestination);
                        removingSource = -1;
                        removingDestination = -1;
                        break;
                    }
                }
            }
            // Проверка на удаление ребра по клику на него
            for (int i = 0; i < numVertices; ++i)
            {
                for (int j = 0; j < numVertices; ++j)
                {
                    int weight = graph.getAdjacencyMatrix()[i][j]; // Получаем матрицу смежности из графа
                    if (weight > 0)
                    {
                        const float angle1 = i * angleStep;
                        const float angle2 = j * angleStep;
                        const int x1 = static_cast<int>(center.x + radius * std::cos(angle1));
                        const int y1 = static_cast<int>(center.y + radius * std::sin(angle1));
                        const int x2 = static_cast<int>(center.x + radius * std::cos(angle2));
                        const int y2 = static_cast<int>(center.y + radius * std::sin(angle2));

                        // Создание объекта Rectangle для проверки клика
                        juce::Rectangle<int> edgeBounds(std::min(x1, x2), std::min(y1, y2), std::abs(x1 - x2), std::abs(y1 - y2));
                        juce::Rectangle<int> expandedBounds = edgeBounds.expanded(4, 4);

                        if (expandedBounds.contains(mousePos))
                        {
                            removeEdge(i, j);
                            break;
                        }
                    }
                }
            }
            addButton.setEnabled(true);
            addEdgeButton.setEnabled(true);
            weightButton.setEnabled(true);
            removeButton.setEnabled(true);
            DFS.setEnabled(true);
            BFS.setEnabled(true);
            Dji.setEnabled(true);
            TSP.setEnabled(true);
        }
        else if (isSettingWeight)
        {
            const juce::Point<int> mousePos = event.getPosition();
            const int numVertices = graph.getAdjacencyMatrix().size();
            const int radius = 200;
            const juce::Point<int> center(getWidth() / 2, getHeight() / 2);
            const float angleStep = 2 * juce::MathConstants<float>::pi / numVertices;
            for (int i = 0; i < numVertices; ++i)
            {
                const float angle = i * angleStep;
                const int x = static_cast<int>(center.x + radius * std::cos(angle)) - vertexSize / 2;
                const int y = static_cast<int>(center.y + radius * std::sin(angle)) - vertexSize / 2;
                juce::Rectangle<int> vertexBounds(x, y, vertexSize, vertexSize);
                if (vertexBounds.contains(mousePos))
                {
                    if (selectedVertex1 == -1)
                    {
                        selectedVertex1 = i;
                    }
                    else if (selectedVertex2 == -1 && i != selectedVertex1)
                    {
                        selectedVertex2 = i;
                        weightInput.setVisible(true);
                        weightInput.toFront(true);
                        weightInput.setSelectAllWhenFocused(true);
                        weightInput.setText(juce::String(weight));
                        weightInput.setWantsKeyboardFocus(true);
                        weightInput.addListener(this);
                    }
                    break;
                }
            }
        }
    }

    void textEditorReturnKeyPressed(juce::TextEditor& editor) override
    {
        if (&editor == &weightInput)
        {
            const juce::String weightText = weightInput.getText();
            weight = weightText.getIntValue();
            graph.setAdjacencyMatrixValue(selectedVertex1, selectedVertex2, weight);
            repaint();
            selectedVertex1 = -1;
            selectedVertex2 = -1;
            weightInput.setVisible(false);
            weightInput.removeListener(this);
            addButton.setEnabled(true);
            removeButton.setEnabled(true);
            addEdgeButton.setEnabled(true);
            removeEdgeButton.setEnabled(true);
            DFS.setEnabled(true);
            BFS.setEnabled(true);
            Dji.setEnabled(true);
            TSP.setEnabled(true);
            isSettingWeight = false;
            weightButton.setButtonText("Set Weight");
        }
        if (&editor == &algorimInput)
        {
            if (dfs)
            {
                const juce::String algorithmText = algorimInput.getText();
                int start = algorithmText.getIntValue();
                --start;
                if (start < 0 || start >= graph.getAdjacencyMatrix().size())
                {
                    algorimInput.setVisible(false);
                    dfs = false;
                    return;
                }

                std::vector<int> input = graph.depthFirstTraversal(start);

                juce::String numberText;
                if (input.size() > 1)
                {
                    for (auto number : input)
                        numberText += juce::String(number + 1) + " ";
                }
                else
                {
                    numberText = "No traversal sequence found.";
                }

                numberField.setBounds(10, 290, 100, 30);
                numberField.setColour(juce::TextEditor::textColourId, juce::Colours::white);
                numberField.setText(numberText);
                addAndMakeVisible(numberField);
                numberField.setVisible(true);

                algorimInput.setVisible(false);
                dfs = false;
            }
            if (bfs)
            {
                const juce::String algorithmText = algorimInput.getText();
                int start = algorithmText.getIntValue();
                --start;
                if (start < 0 || start >= graph.getAdjacencyMatrix().size())
                {
                    algorimInput.setVisible(false);
                    bfs = false;
                    return;
                }

                std::vector<int> input = graph.breadthFirstTraversal(start);

                juce::String numberText;
                if (input.size() > 1)
                {
                    for (auto number : input)
                        numberText += juce::String(number + 1) + " ";
                }
                else
                {
                    numberText = "Error!";
                }
                numberField.setBounds(10, 290, 100, 30);
                numberField.setColour(juce::TextEditor::textColourId, juce::Colours::white);
                numberField.setText(numberText);
                addAndMakeVisible(numberField);
                numberField.setVisible(true);

                algorimInput.setVisible(false);
                bfs = false;
            }
            if (dji)
            {
                const juce::String algorithmText = algorimInput.getText();
                int start = algorithmText.getIntValue();
                --start;
                if (start < 0 || start >= graph.getAdjacencyMatrix().size())
                {
                    algorimInput.setVisible(false);
                    dji = false;
                    return;
                }

                std::vector<std::pair<int, int>> input = graph.dijkstraAlgorithm(start);

                juce::String numberText;
                if (input.size() > 1)
                {
                    for (auto pair : input)
                    {
                        numberText += juce::String(pair.first + 1) + " (" + juce::String(pair.second) + ") ";
                    }
                }
                else
                {
                    numberText = "No found.";
                }

                numberField.setBounds(10, 560, 250, 30);
                numberField.setColour(juce::TextEditor::textColourId, juce::Colours::white);
                addAndMakeVisible(numberField);
                numberField.setText(numberText);
                numberField.setVisible(true);

                algorimInput.setVisible(false);
                dji = false;
            }
        }
    }

private:
    static const int vertexSize = 40;
    static const int maxVertices = 6;
    const std::vector<juce::Colour> vertexColors = {
        juce::Colours::blue,
        juce::Colours::red,
        juce::Colours::green,
        juce::Colours::orange,
        juce::Colours::purple,
        juce::Colours::yellow
    };
    Graph_logics graph;
    juce::TextButton addButton;
    juce::TextButton removeButton;
    juce::TextButton addEdgeButton;
    juce::TextButton removeEdgeButton;
    juce::TextButton weightButton;
    juce::TextButton DFS;
    juce::TextButton BFS;
    juce::TextButton Dji;
    juce::TextButton TSP;
    juce::TextEditor weightInput;
    juce::TextEditor algorimInput;
    juce::TextEditor numberField;
    int sourceVertex = -1;
    int destinationVertex = -1;
    int removingSource = -1;
    int removingDestination = -1;
    int changingSource = -1;
    int changingDestination = -1;
    int selectedVertex1 = -1;
    int selectedVertex2 = -1; 
    int chooseint = 0;
    int weight = 0; 
    bool isSettingWeight = false;
    bool isRemovingEdge = false;
    bool isRemovingVertex = false;
    bool isAddingEdge = false;
    bool bfs = false;
    bool dfs = false;
    bool dji = false;
    bool tsp = false;
};

class MainComponent : public juce::Component
{
public:
    MainComponent()
    {
        setSize(400, 400);
        addAndMakeVisible(graphComponent);
    }

    void paint(juce::Graphics& g) override
    {
    }

    void resized() override
    {
        graphComponent.setBounds(getLocalBounds());
    }

private:
    Graph_logics graph;
    GraphComponent graphComponent;
};

class GraphApplication : public juce::JUCEApplication
{
public:
    GraphApplication() {}
    void initialise(const juce::String& commandLine) override
    {
        mainWindow.reset(new MainWindow(getApplicationName()));
        mainWindow->setContentOwned(new MainComponent(), true);
        mainWindow->centreWithSize(800, 600);
        mainWindow->setVisible(true);
    }
    void shutdown() override { mainWindow = nullptr; }
    const juce::String getApplicationName() override { return "Graph Visualization"; }
    const juce::String getApplicationVersion() override { return "1.0.0"; }
    bool moreThanOneInstanceAllowed() override { return true; }
    void systemRequestedQuit() override { quit(); }
    void anotherInstanceStarted(const juce::String& commandLine) override {}
    class MainWindow : public juce::DocumentWindow
    {
    public:
        MainWindow(juce::String name)
            : DocumentWindow(name,
                juce::Desktop::getInstance().getDefaultLookAndFeel()
                .findColour(juce::ResizableWindow::backgroundColourId),
                DocumentWindow::closeButton)
        {
            setUsingNativeTitleBar(true);
            setContentOwned(new MainComponent(), true);
            #if JUCE_IOS || JUCE_ANDROID
            setFullScreen(true);
            #else
            setResizable(false, false);
            centreWithSize(getWidth(), getHeight());
            #endif
            setVisible(true);
        }
        void closeButtonPressed() override { JUCEApplication::getInstance()->systemRequestedQuit(); }
    private: JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainWindow)
    };
private: std::unique_ptr<MainWindow> mainWindow;
};

START_JUCE_APPLICATION(GraphApplication)