#include <JuceHeader.h>
#include <random>

class BinaryTreeView : public juce::Component, public juce::Button::Listener, public juce::MouseListener, public juce::TextEditor::Listener
{
public:
    BinaryTreeView()
    {
        rootNode = nullptr;
        addButton.setButtonText("Add Node");
        addButton.addListener(this);
        addAndMakeVisible(addButton);
        removeButton.setButtonText("Del Node");
        removeButton.addListener(this);
        addAndMakeVisible(removeButton);
        balanceButton.setButtonText("Balance");
        balanceButton.addListener(this);
        addAndMakeVisible(balanceButton);
        addMouseListener(this, true);
        nodeValueInput.setEnabled(false);
        nodeValueInput.addListener(this);
        addAndMakeVisible(nodeValueInput);
    }

    ~BinaryTreeView() override { deleteTree(rootNode); }

    void paint(juce::Graphics& g) override
    {
        g.fillAll(juce::Colours::white);
        if (rootNode != nullptr)
        {
            levelWidth = getWidth() / 2;
            int startX = getWidth() / 2;
            int startY = 50;
            int levelHeight = 100;
            int nodeSize = 35;
            drawTree(g, startX, startY, rootNode, nodeSize, levelHeight, levelWidth);
        }
    }

    void resized() override
    {
        addButton.setBounds(10, 10, 80, 30);
        removeButton.setBounds(10, 45, 80, 30);
        balanceButton.setBounds(100, 10, 80, 30);
        nodeValueInput.setBounds(100, 45, 30, 30);
    }

    void buttonClicked(juce::Button* button) override
    {
    if (button == &removeButton)
    {
        if (!nodeValueInput.isEnabled())
        {
            nodeValueInput.setEnabled(true);
            nodeValueInput.toFront(true);
            nodeValueInput.grabKeyboardFocus();
        }
        else
        {
            removeNode();
            nodeValueInput.setEnabled(false);
            nodeValueInput.clear();
        }
    }
        else if (button == &addButton)
        {
            std::random_device rd;
            std::mt19937 rng(rd());
            std::uniform_int_distribution<int> uni(0, 100);
            int value = uni(rng);
            if (isValueUnique(value)) insertNode(rootNode, value);
        }
        else if (button == &balanceButton)
        {
            balanceTree();
        }
        repaint();
    }

private:
    struct Node
    {
        int value;
        juce::Colour colour;
        Node* left;
        Node* right;
        Node(int value) : value(value), left(nullptr), right(nullptr)
        {
            std::random_device rd;
            std::mt19937 rng(rd());
            std::uniform_int_distribution<int> uni(0, 255);
            colour = juce::Colour(uni(rng), uni(rng), uni(rng));
        }
    };

    Node* rootNode;
    Node* selectedNode;
    juce::TextButton addButton;
    juce::TextButton removeButton;
    juce::TextButton balanceButton;
    juce::TextEditor nodeValueInput;
    int levelWidth;

    void removeNode()
    {
        int valueToRemove = nodeValueInput.getText().getIntValue();
        Node* nodeToRemove = findNode(rootNode, valueToRemove);
        if (nodeToRemove != nullptr)
        {
            deleteNode(rootNode, valueToRemove);
            repaint();
        }
    }

    void textEditorReturnKeyPressed(juce::TextEditor& editor) override
    {
        if (&editor == &nodeValueInput)
        {
            removeNode();
            nodeValueInput.setEnabled(false);
            nodeValueInput.clear();
        }
    }

    void deleteNode(Node*& node, int value)
    {
        if (node == nullptr)
            return;

        if (value < node->value)
        {
            deleteNode(node->left, value);
        }
        else if (value > node->value)
        {
            deleteNode(node->right, value);
        }
        else
        {
            if (node->left == nullptr && node->right == nullptr)
            {
                delete node;
                node = nullptr;
            }
            else if (node->left == nullptr)
            {
                Node* temp = node;
                node = node->right;
                delete temp;
            }
            else if (node->right == nullptr)
            {
                Node* temp = node;
                node = node->left;
                delete temp;
            }
            else
            {
                Node* successor = findSuccessor(node->right);
                node->value = successor->value;
                deleteNode(node->right, successor->value);
            }
        }
    }

    Node* findSuccessor(Node* node)
    {
        if (node->left == nullptr)
            return node;

        return findSuccessor(node->left);
    }

    void deleteTree(Node* node)
    {
        if (node != nullptr)
        {
            deleteTree(node->left);
            deleteTree(node->right);
            delete node;
        }
    }

    void drawTree(juce::Graphics& g, int x, int y, Node* node, int nodeSize, int levelHeight, int levelWidth)
    {
        if (node != nullptr)
        {
            int nodeX = x - nodeSize / 2;
            int nodeY = y - nodeSize / 2;

            if (node->left != nullptr)
            {
                int childX = x - levelWidth / 2;
                int childY = y + levelHeight;

                g.setColour(juce::Colours::black);
                if (y >= childY)
                {
                    // Adjust line color in the region where it overlaps the node
                    int overlapY = y - childY;
                    g.drawLine(x, y - nodeSize / 2, childX, childY + overlapY);
                    g.setColour(node->colour);
                    g.fillEllipse(nodeX, nodeY, nodeSize, nodeSize);
                    g.setColour(juce::Colours::black);
                    g.drawLine(x, y - nodeSize / 2 + overlapY, childX, childY);
                }
                else
                {
                    g.drawLine(x, y, childX, childY);
                }
                drawTree(g, childX, childY, node->left, nodeSize, levelHeight, levelWidth / 2);
            }

            if (node->right != nullptr)
            {
                int childX = x + levelWidth / 2;
                int childY = y + levelHeight;

                g.setColour(juce::Colours::black);
                if (y >= childY)
                {
                    int overlapY = y - childY;
                    g.drawLine(x, y - nodeSize / 2, childX, childY + overlapY);
                    g.setColour(node->colour);
                    g.fillEllipse(nodeX, nodeY, nodeSize, nodeSize);
                    g.setColour(juce::Colours::black);
                    g.drawLine(x, y - nodeSize / 2 + overlapY, childX, childY);
                }
                else
                {
                    g.drawLine(x, y, childX, childY);
                }
                drawTree(g, childX, childY, node->right, nodeSize, levelHeight, levelWidth / 2);
            }

            g.setColour(node->colour);
            g.fillEllipse(nodeX, nodeY, nodeSize, nodeSize);

            juce::String valueString = juce::String(node->value);
            g.setColour(juce::Colours::white);
            g.setFont(nodeSize * 0.7f);
            g.drawText(valueString, nodeX, nodeY, nodeSize, nodeSize, juce::Justification::centred);
        }
    }

    bool isValueUnique(int value) { return !findNode(rootNode, value); }

    Node* findNode(Node* node, int value)
    {
        if (node == nullptr || node->value == value) return node;
        if (value < node->value) return findNode(node->left, value);
        return findNode(node->right, value);
    }

    void insertNode(Node*& node, int value)
    {
        if (node == nullptr) node = new Node(value);
        else if (value < node->value) insertNode(node->left, value);
        else insertNode(node->right, value);
    }

    void balanceTree()
    {
        juce::Array<int> values;
        treeToArray(rootNode, values);

        deleteTree(rootNode);

        rootNode = arrayToTree(values, 0, values.size() - 1);
    }

    void treeToArray(Node* node, juce::Array<int>& values)
    {
        if (node != nullptr)
        {
            treeToArray(node->left, values);
            values.add(node->value);
            treeToArray(node->right, values);
        }
    }

    Node* arrayToTree(const juce::Array<int>& values, int start, int end)
    {
        if (start > end)
            return nullptr;

        int mid = (start + end) / 2;
        Node* node = new Node(values[mid]);

        node->left = arrayToTree(values, start, mid - 1);
        node->right = arrayToTree(values, mid + 1, end);

        return node;
    }
};

class MainComponent : public juce::Component
{
public:
    MainComponent()
    {
        addAndMakeVisible(binaryTreeView);
    }

    void resized() override
    {
        binaryTreeView.setBounds(getLocalBounds());
    }

private:
    BinaryTreeView binaryTreeView;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};

class GraphApplication : public juce::JUCEApplication
{
public:
    GraphApplication() {}
    void initialise(const juce::String& commandLine) override
    {
        mainWindow.reset(new MainWindow(getApplicationName()));
        mainWindow->setContentOwned(new MainComponent(), true);
        mainWindow->setFullScreen(true);
        mainWindow->setResizeLimits(800, 600, 1920, 1080);
        mainWindow->setIcon(juce::ImageFileFormat::loadFrom(BinaryData::icon_png, BinaryData::icon_pngSize));
        mainWindow->setVisible(true);
    }
    void shutdown() override { mainWindow = nullptr; }
    const juce::String getApplicationName() override { return "Tree Visualization - by Karrton"; }
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
                DocumentWindow::allButtons)
        {
            setUsingNativeTitleBar(true);
            setContentOwned(new MainComponent(), true);
#if JUCE_IOS || JUCE_ANDROID
            setFullScreen(true);
#else
            setResizable(true, true);
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