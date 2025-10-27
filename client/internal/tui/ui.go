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
}

var help_style = lipgloss.NewStyle().Foreground(lipgloss.Color("205")).Render

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
	case serverMsg:
		m.message = msg.text
	case errMsg:
		m.err = msg.err

	case tea.KeyMsg:
		if msg.String() == "q" {
			return m, tea.Quit
		}
		// switch msg.Type {
		// case tea.KeyCtrlC, tea.KeyEsc:
		// 	return m, tea.Quit
		// }
	}
	return m, nil
}

var docStyle = lipgloss.NewStyle().Margin(1, 2).BorderStyle(lipgloss.RoundedBorder()).BorderForeground(lipgloss.Color("63"))

func (m model) View() string {
	if m.err != nil {
		return fmt.Sprintf("Error: %v\n", m.err)
	} else {
		return docStyle.Render(fmt.Sprintf("Message: %s\n %s\n", m.message, m.help_view()))
	}
}

func (m model) help_view() string {
	return help_style("\n  ↑/↓: Navigate • q: Quit\n")
}

func NewModel() model { return model{} }
