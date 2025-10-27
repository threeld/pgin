package tui

import (
	"fmt"

	"github.com/ThreeLD/pgin/client/internal"
	tea "github.com/charmbracelet/bubbletea"
	"github.com/charmbracelet/lipgloss"
)

type serverMsg struct{ text string }
type errMsg struct{ err error }

type model struct {
	message string
	err     error
	width   int
	height  int
}

var helpStyle = lipgloss.NewStyle().
	Foreground(lipgloss.Color("86")).
	Render

func (m model) Init() tea.Cmd {
	return func() tea.Msg {
		msg, err := internal.Msg()
		if err != nil {
			return errMsg{err}
		}
		return serverMsg{msg}
	}
}

func (m model) Update(msg tea.Msg) (tea.Model, tea.Cmd) {
	switch msg := msg.(type) {
	case tea.WindowSizeMsg:
		m.width = msg.Width
		m.height = msg.Height
	case serverMsg:
		m.message = msg.text
	case errMsg:
		m.err = msg.err

	case tea.KeyMsg:
		if msg.String() == "q" {
			return m, tea.Quit
		}
	}
	return m, nil
}

func (m model) View() string {
	if m.err != nil {
		return fmt.Sprintf("Error: %v\n", m.err)
	}

	// style for the message box
	boxStyle := lipgloss.NewStyle().
		Border(lipgloss.RoundedBorder()).
		BorderForeground(lipgloss.Color("#808080")).
		Width(m.width-3).
		Height(m.height-5).
		Padding(0, 2)

	// message from the server
	messageContent := fmt.Sprintf("Message: %s", m.message)

	// render the message box
	messageBox := boxStyle.Render(messageContent)

	// join vertically the message box and the help view
	return lipgloss.JoinVertical(lipgloss.Left, messageBox, m.help_view())
}

func (m model) help_view() string {
	return helpStyle("\n  ↑/↓: Navigate • q: Quit\n")
}

func NewModel() model { return model{} }
